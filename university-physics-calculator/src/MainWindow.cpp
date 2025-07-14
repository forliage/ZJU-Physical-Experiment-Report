#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "Calculator.h" 
#include <QMessageBox>  
#include <QFileDialog>   
#include <QFile>        
#include <QTextStream>   
#include <QDebug>  

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->dataTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::readDataFromTable() {
    m_reportData.data = DataSet(); 
    for (int row = 0; row < ui->dataTable->rowCount(); ++row) {
        QTableWidgetItem *itemX = ui->dataTable->item(row, 0);
        QTableWidgetItem *itemY = ui->dataTable->item(row, 1);

        if (itemX && !itemX->text().isEmpty() && itemY && !itemY->text().isEmpty()) {
            bool okX, okY;
            double x = itemX->text().toDouble(&okX);
            double y = itemY->text().toDouble(&okY);
            if (okX && okY) {
                m_reportData.data.addPoint(x, y);
            }
        }
    }
}


void MainWindow::on_addPointButton_clicked() {
    ui->dataTable->insertRow(ui->dataTable->rowCount());
}

void MainWindow::on_clearDataButton_clicked() {
    ui->dataTable->clearContents(); 
    ui->dataTable->setRowCount(10); 
    ui->resultsTextEdit->clear(); 
    m_reportData = ReportData(); 
}

void MainWindow::on_calculateStatsButton_clicked() {
    readDataFromTable();
    if (m_reportData.data.size() < 2) {
        QMessageBox::warning(this, "错误", "数据点不足，无法进行统计计算。");
        return;
    }

    m_reportData.stats = Calculator::calculateStatistics(m_reportData.data.getYValues());
    m_reportData.statsCalculated = true;
    m_reportData.uncertaintyA = Calculator::calculateUncertaintyA(m_reportData.data);

    QString resultText;
    QTextStream ss(&resultText);
    ss.setRealNumberPrecision(5);
    ss << "--- 统计结果 ---\n"
       << "数据点数量: " << m_reportData.stats.count << "\n"
       << "平均值: " << m_reportData.stats.mean << "\n"
       << "标准偏差: " << m_reportData.stats.std_dev << "\n"
       << "方差: " << m_reportData.stats.variance << "\n\n"
       << "--- 不确定度初算 ---\n"
       << "A类不确定度 (uA): " << m_reportData.uncertaintyA;
    
    ui->resultsTextEdit->setText(resultText);
    ui->uncertaintyALineEdit->setText(QString::number(m_reportData.uncertaintyA, 'f', 5));
}


void MainWindow::on_linearFitButton_clicked() {
    readDataFromTable();
    if (m_reportData.data.size() < 2) {
        QMessageBox::warning(this, "错误", "数据点不足，无法进行线性拟合。");
        return;
    }

    m_reportData.linearFit = Calculator::linearFit(m_reportData.data);
    m_reportData.linearFitCalculated = true;
    
    QString resultText = ui->resultsTextEdit->toPlainText();
    QTextStream ss(&resultText);
    ss.setRealNumberPrecision(5);
    ss << "\n\n--- 线性拟合结果 ---\n"
       << "方程: y = " << m_reportData.linearFit.slope_a << " * x + " << m_reportData.linearFit.intercept_b << "\n"
       << "相关系数 (R²): " << m_reportData.linearFit.r_squared;

    ui->resultsTextEdit->setText(resultText);
}

void MainWindow::on_polyFitButton_clicked() {
    readDataFromTable();
    int order = ui->polyOrderSpinBox->value();
    if (m_reportData.data.size() < static_cast<size_t>(order + 1)) {
        QMessageBox::warning(this, "错误", QString("数据点不足，%1阶拟合至少需要%2个点。").arg(order).arg(order + 1));
        return;
    }
    
    try {
        m_reportData.polyCoeffs = Calculator::polynomialFit(m_reportData.data, order);
        m_reportData.polyOrder = order;
        m_reportData.polyFitCalculated = true;

        QString resultText = ui->resultsTextEdit->toPlainText();
        QTextStream ss(&resultText);
        ss.setRealNumberPrecision(5);
        ss << "\n\n--- " << order << "阶多项式拟合结果 ---\n"
           << "系数 (c0, c1, ...): ";
        for(int i = 0; i < m_reportData.polyCoeffs.size(); ++i) {
            ss << m_reportData.polyCoeffs[i] << (i == m_reportData.polyCoeffs.size() - 1 ? "" : ", ");
        }
        ui->resultsTextEdit->setText(resultText);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "拟合失败", e.what());
    }
}


void MainWindow::on_calculateUncertaintyButton_clicked() {
    bool ok;
    double uB = ui->uncertaintyBLineEdit->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "输入错误", "请输入有效的B类不确定度数值。");
        return;
    }
    
    if (!m_reportData.statsCalculated) {
        QMessageBox::information(this, "提示", "请先在“数据与统计”选项卡中点击“计算统计量”来获取A类不确定度。");
        return;
    }

    m_reportData.uncertaintyB = uB;
    m_reportData.totalUncertainty = Calculator::calculateTotalUncertainty(m_reportData.uncertaintyA, m_reportData.uncertaintyB);
    m_reportData.uncertaintyCalculated = true;

    ui->totalUncertaintyLineEdit->setText(QString::number(m_reportData.totalUncertainty, 'f', 5));
}

void MainWindow::saveReport(const std::string& format) {
    QString filter;
    std::string reportContent;

    if (format == "md") {
        filter = "Markdown 文件 (*.md)";
        reportContent = ReportGenerator::generateMarkdown(m_reportData);
    } else if (format == "typ") {
        filter = "Typst 文件 (*.typ)";
        reportContent = ReportGenerator::generateTypst(m_reportData);
    } else {
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "保存报告", "", filter);
    if (fileName.isEmpty()) {
        return; 
    }
    
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::fromStdString(reportContent);
        file.close();
        QMessageBox::information(this, "成功", "报告已成功保存！");
    } else {
        QMessageBox::critical(this, "错误", "无法保存文件。请检查路径和权限。");
    }
}

void MainWindow::on_saveMarkdownButton_clicked() {
    readDataFromTable(); 
    saveReport("md");
}

void MainWindow::on_saveTypstButton_clicked() {
    readDataFromTable(); 
    saveReport("typ");
}