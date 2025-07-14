#pragma once

#include <QMainWindow>
#include "ReportGenerator.h" 

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addPointButton_clicked();
    void on_clearDataButton_clicked();
    void on_calculateStatsButton_clicked();
    void on_linearFitButton_clicked();
    void on_polyFitButton_clicked();
    void on_calculateUncertaintyButton_clicked();
    void on_saveMarkdownButton_clicked();
    void on_saveTypstButton_clicked();

private:
    Ui::MainWindow *ui;
    ReportData m_reportData;

    void readDataFromTable(); 
    void saveReport(const std::string& format);
};