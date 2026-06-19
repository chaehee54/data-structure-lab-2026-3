#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QListWidget>
#include <QMouseEvent>
#include "graph.h"

// ── 지도 위젯 (공항 노드 + 노선 그리기) ──
class MapWidget : public QLabel {
    Q_OBJECT
public:
    explicit MapWidget(QWidget* parent = nullptr);
    void setGraph(const FlightGraph* graph);
    void setSelected(const QString& code);

signals:
    void airportClicked(const QString& code);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    const FlightGraph* m_graph    = nullptr;
    QString            m_selected;
    static const int   RADIUS     = 10;
};

// ── 메인 윈도우 ──
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAirportClicked(const QString& code);
    void onFlightClicked(QListWidgetItem* item);

private:
    void buildGraph();
    void setupUI();
    void refreshPanel(const QString& code);

    FlightGraph  m_graph;

    MapWidget*   m_map;
    QLabel*      m_titleLabel;
    QLabel*      m_subLabel;
    QListWidget* m_list;
};