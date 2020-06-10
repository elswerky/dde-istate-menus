#include "istatecpuwidget.h"
#include "ui_istatecpuwidget.h"

IstateCpuWidget::IstateCpuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IstateCpuWidget)
{
    ui->setupUi(this);

    this->cpuIdleColor = QColor("#404244");
    this->cpuUserColor = QColor("#5BBDF8");
    this->cpuUserAreaColor = QColor("#296085");
    this->cpuSystemColor = QColor("#EA654D");
    this->cpuSystemAreaColor = QColor("#811D18");

    this->loadIdleColor = QColor("#9C9CA0");
    this->loadUserColor = this->cpuUserColor;
    this->loadSystemColor = this->cpuSystemColor;

    ui->cpuIdleColorLabel->setStyleSheet(QString("QLabel { background-color: %1; }").arg(this->cpuIdleColor.name()));
    ui->cpuSystemColorLabel->setStyleSheet(QString("QLabel { background-color: %1; }").arg(this->cpuSystemColor.name()));
    ui->cpuUserColorLabel->setStyleSheet(QString("QLabel { background-color: %1; }").arg(this->cpuUserColor.name()));

    initCpuChart();
    initCpuBarChart();
    initLoadChart();
}

IstateCpuWidget::~IstateCpuWidget()
{
    delete ui;
}

void IstateCpuWidget::initCpuChart() {
    this->m_cpuChart = new QChart();

    this->m_userSeries = new QLineSeries();
    this->m_userSeries->setPen(QPen(this->cpuUserColor, 2));
    this->m_userSeries->setBrush(Qt::NoBrush);

    this->m_systemSeries = new QLineSeries();
    this->m_systemSeries->setPen(QPen(this->cpuSystemColor, 2));
    this->m_systemSeries->setBrush(Qt::NoBrush);

    this->m_cpuZeroSeries = new QLineSeries();
    this->m_cpuZeroSeries->setPen(Qt::NoPen);
    this->m_cpuZeroSeries->setBrush(Qt::NoBrush);
    this->m_cpuChart->addSeries(this->m_cpuZeroSeries);

    for (int i = 0; i < this->maxHistorySize; ++i) {
        this->m_cpuZeroSeries->append(i, 0);
    }

    this->m_userAreaSeries = new QAreaSeries(this->m_userSeries, this->m_cpuZeroSeries);
    this->m_userAreaSeries->setPen(QPen(Qt::transparent));
    this->m_userAreaSeries->setBrush(this->cpuUserAreaColor);
    this->m_cpuChart->addSeries(this->m_userAreaSeries);

    this->m_systemAreaSeries = new QAreaSeries(this->m_systemSeries, this->m_userSeries);
    this->m_systemAreaSeries->setPen(QPen(Qt::transparent));
    this->m_systemAreaSeries->setBrush(this->cpuSystemAreaColor);
    this->m_cpuChart->addSeries(this->m_systemAreaSeries);

    this->m_cpuChart->addSeries(this->m_userSeries);
    this->m_cpuChart->addSeries(this->m_systemSeries);

    // enable the legend and move it to bottom
    this->m_cpuChart->legend()->setVisible(false);

    // set the default range of the axes and hide the axes, grid line, and label
    this->m_cpuChart->createDefaultAxes();
    this->m_cpuChart->axisX()->setRange(0, this->maxHistorySize - 1);
    this->m_cpuChart->axisX()->setGridLineVisible(false);
    this->m_cpuChart->axisX()->setLabelsVisible(false);
    this->m_cpuChart->axisX()->setVisible(false);

    this->m_cpuChart->axisY()->setRange(0, 100);
    this->m_cpuChart->axisY()->setGridLineVisible(false);
    this->m_cpuChart->axisY()->setLabelsVisible(false);
    this->m_cpuChart->axisY()->setVisible(false);

    this->m_cpuChart->setMargins(QMargins(0, 0, 0, 0));
    this->m_cpuChart->layout()->setContentsMargins(0, 0, 0, 0);
    this->m_cpuChart->setBackgroundBrush(QBrush(this->cpuIdleColor));
    this->m_cpuChart->setBackgroundRoundness(0);

    ui->cpuChartView->setChart(this->m_cpuChart);
    ui->cpuChartView->setContentsMargins(0, 0, 0, 0);
    ui->cpuChartView->setRenderHint(QPainter::Antialiasing);
}

void IstateCpuWidget::initLoadChart() {
    this->m_loadChart = new QChart();

    this->m_loadUserSeries = new QLineSeries();
    this->m_loadUserSeries->setPen(QPen(this->loadUserColor, 2));
    this->m_loadUserSeries->setBrush(Qt::NoBrush);
    this->m_loadChart->addSeries(this->m_loadUserSeries);

    this->m_loadSystemSeries = new QLineSeries();
    this->m_loadSystemSeries->setPen(QPen(this->loadSystemColor, 2));
    this->m_loadSystemSeries->setBrush(Qt::NoBrush);
    this->m_loadChart->addSeries(this->m_loadSystemSeries);

    this->m_loadIdleSeries = new QLineSeries();
    this->m_loadIdleSeries->setPen(QPen(this->loadIdleColor, 2));
    this->m_loadIdleSeries->setBrush(Qt::NoBrush);
    this->m_loadChart->addSeries(this->m_loadIdleSeries);

    this->m_loadChart->legend()->setVisible(false);

    // set the default range of the axes and hide the axes, grid line, and label
    this->m_loadChart->createDefaultAxes();
    this->m_loadChart->axisX()->setRange(0, this->maxHistorySize - 1);
    this->m_loadChart->axisX()->setGridLineVisible(false);
    this->m_loadChart->axisX()->setLabelsVisible(false);
    this->m_loadChart->axisX()->setVisible(false);

    this->m_loadChart->axisY()->setRange(0, 10);
    this->m_loadChart->axisY()->setGridLineVisible(false);
    this->m_loadChart->axisY()->setLabelsVisible(false);
    this->m_loadChart->axisY()->setVisible(false);

    this->m_loadChart->setMargins(QMargins(0, 0, 0, 0));
    this->m_loadChart->layout()->setContentsMargins(0, 0, 0, 0);
    this->m_loadChart->setBackgroundBrush(QBrush(this->cpuIdleColor));
    this->m_loadChart->setBackgroundRoundness(0);

    ui->loadChartView->setChart(this->m_loadChart);
    ui->loadChartView->setContentsMargins(0, 0, 0, 0);
    ui->loadChartView->setRenderHint(QPainter::Antialiasing);

    ui->peakLoadLabel->setStyleSheet(QString("QLabel { background-color: %1; color: white; }").arg(this->cpuIdleColor.name()));
}

void IstateCpuWidget::addCpuUsage(cpu_usage avgUsage, QList<cpu_usage> cpuUsageList) {
    this->cpuUsageList.append(avgUsage);
    if (this->cpuUsageList.size() > this->maxHistorySize) {
        this->cpuUsageList = this->cpuUsageList.mid(this->cpuUsageList.size() - this->maxHistorySize);
    }

    this->cpuUsageBarList = cpuUsageList;
    this->redrawCpuCurve();
    this->redrawCpuBarCurve();
}

void IstateCpuWidget::redrawCpuCurve() {
    if (this->isHidden()) return;  // reduce useless cpu usage

    this->m_userSeries->clear();
    this->m_systemSeries->clear();

    int i;
    for (i = 0; i < this->maxHistorySize - this->cpuUsageList.size(); ++i) {
        this->m_userSeries->append(i, 0);
        this->m_systemSeries->append(i, 0);
    }
    for (int j = 0; j < this->cpuUsageList.size(); ++j) {
        this->m_userSeries->append(i + j, 100 * this->cpuUsageList[j].user);
        this->m_systemSeries->append(i + j, 100 * (this->cpuUsageList[j].system + this->cpuUsageList[j].user));
    }

    ui->cpuUserLabel->setText(QString("%1%").arg(QString::number(this->cpuUsageList.last().user * 100, 'f', 1)));
    ui->cpuSystemLabel->setText(QString("%1%").arg(QString::number(this->cpuUsageList.last().system * 100, 'f', 1)));
    ui->cpuIdleLabel->setText(QString("%1%").arg(QString::number(this->cpuUsageList.last().idle * 100, 'f', 1)));
}

void IstateCpuWidget::showEvent(QShowEvent *event) {
    this->redrawCpuCurve();
    this->redrawCpuBarCurve();
    this->redrawProcesses();
    this->redrawUptime();
    QWidget::showEvent(event);
}

void IstateCpuWidget::initCpuBarChart() {
    this->m_cpuBarChart = new QChart();

    this->m_cpuUserBarSet = new QBarSet("User");
    this->m_cpuUserBarSet->setPen(QPen(this->cpuUserColor, 1));
    this->m_cpuUserBarSet->setBrush(this->cpuUserAreaColor);

    this->m_cpuSystemBarSet = new QBarSet("System");
    this->m_cpuSystemBarSet->setPen(QPen(this->cpuSystemColor, 1));
    this->m_cpuSystemBarSet->setBrush(this->cpuSystemAreaColor);

    this->m_cpuIdleBarSet = new QBarSet("Idle");
    this->m_cpuIdleBarSet->setPen(Qt::NoPen);
    this->m_cpuIdleBarSet->setBrush(Qt::NoBrush);

    this->m_cpuBarSeries = new QPercentBarSeries(this->m_cpuBarChart);
    this->m_cpuBarSeries->append(this->m_cpuUserBarSet);
    this->m_cpuBarSeries->append(this->m_cpuSystemBarSet);
    this->m_cpuBarSeries->append(this->m_cpuIdleBarSet);
    this->m_cpuBarChart->addSeries(this->m_cpuBarSeries);

    QValueAxis *axisY = new QValueAxis();
    axisY->hide();
    axisY->setGridLineVisible(false);
    this->m_cpuBarChart->addAxis(axisY, Qt::AlignLeft);
    this->m_cpuBarSeries->attachAxis(axisY);

    this->m_cpuBarChart->legend()->setVisible(false);

    this->m_cpuBarChart->setMargins(QMargins(0, 0, 0, 0));
    this->m_cpuBarChart->layout()->setContentsMargins(0, 0, 0, 0);
    this->m_cpuBarChart->setBackgroundBrush(QBrush(this->cpuIdleColor));
    this->m_cpuBarChart->setBackgroundRoundness(0);

    ui->cpuBarChartView->setChart(this->m_cpuBarChart);
    ui->cpuBarChartView->setContentsMargins(0, 0, 0, 0);
    ui->cpuBarChartView->setRenderHint(QPainter::Antialiasing);
}

void IstateCpuWidget::redrawCpuBarCurve() {
    if (this->isHidden()) return;

    if (this->categories.size() != this->cpuUsageBarList.size()) {
        this->categories.clear();
        for (int i  = 0; i < this->cpuUsageBarList.size(); ++i) {
            categories << QString::number(i);
        }
        ui->cpuBarChartView->setFixedWidth(10 * this->cpuUsageBarList.size() + 2);

        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->hide();
        axisX->append(categories);
        this->m_cpuBarChart->addAxis(axisX, Qt::AlignBottom);
        this->m_cpuBarSeries->attachAxis(axisX);
        this->m_cpuBarSeries->setBarWidth((this->cpuUsageBarList.size() - 1.0) / this->cpuUsageBarList.size());

    }

    this->m_cpuUserBarSet->remove(0, this->cpuUsageBarList.size());
    this->m_cpuSystemBarSet->remove(0, this->cpuUsageBarList.size());
    this->m_cpuIdleBarSet->remove(0, this->cpuUsageBarList.size());

    for (auto & i : this->cpuUsageBarList) {
        this->m_cpuUserBarSet->append(i.user);
        this->m_cpuSystemBarSet->append(i.system);
        this->m_cpuIdleBarSet->append(i.idle);
    }
}

void IstateCpuWidget::updateProcesses(QList<ProcessEntry> entryList) {
    std::sort(entryList.begin(), entryList.end(), [](const ProcessEntry &e1, const ProcessEntry &e2) {
        return e1.getCPU() > e2.getCPU();
    });
    this->entries = entryList.mid(0, 5);
    this->redrawProcesses();
}

void IstateCpuWidget::redrawProcesses() {
    if (this->isHidden()) return;

    int r;
    for (r = 0; r < ui->processGridLayout->rowCount() && r < this->entries.size(); ++r) {
        QLabel *label = dynamic_cast<QLabel*>(ui->processGridLayout->itemAtPosition(r, 0)->widget());
        label->setPixmap(this->entries[r].getIcon().pixmap(label->size()));

        label = dynamic_cast<QLabel*>(ui->processGridLayout->itemAtPosition(r, 1)->widget());
        label->setText(this->entries[r].getName());

        label = dynamic_cast<QLabel*>(ui->processGridLayout->itemAtPosition(r, 2)->widget());
        label->setText(QString("%1%").arg(QString::number(this->entries[r].getCPU(), 'f', 1)));
    }

    for (; r < ui->processGridLayout->rowCount(); ++r) {
        QLabel *label = dynamic_cast<QLabel*>(ui->processGridLayout->itemAtPosition(r, 0)->widget());
        label->clear();

        label = dynamic_cast<QLabel*>(ui->processGridLayout->itemAtPosition(r, 1)->widget());
        label->clear();

        label = dynamic_cast<QLabel*>(ui->processGridLayout->itemAtPosition(r, 2)->widget());
        label->clear();
    }
}

void IstateCpuWidget::updateUptime(qulonglong uptime) {
    this->uptime = uptime;
    this->redrawUptime();
}

void IstateCpuWidget::redrawUptime() {
    if (this->isHidden()) return;

    int minutes = this->uptime / 60;
    int nowMinutes = minutes % 60;
    int hours = minutes / 60;
    int nowHours = hours % 24;
    int days = hours / 24;

    ui->uptimeLabel->setText(QString("%1 days, %2 hours, %3 minutes").arg(days).arg(nowHours).arg(nowMinutes));
}