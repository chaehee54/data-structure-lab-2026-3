#include "MainWindow.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QFont>

// ════════════════════════════════════════════
//  MapWidget
// ════════════════════════════════════════════

MapWidget::MapWidget(QWidget* parent) : QLabel(parent) {
    setCursor(Qt::PointingHandCursor);

    QPixmap pm("worldmap.jpg");
    if (pm.isNull()) {
        pm = QPixmap(800, 400);
        pm.fill(QColor("#0d1b2a"));
        QPainter p(&pm);
        p.setPen(Qt::white);
        p.drawText(pm.rect(), Qt::AlignCenter,
                   "worldmap.jpg 파일을 프로젝트 폴더에 넣어주세요!");
    }
    // 이미지를 800x400 으로 표시 (원본 비율 유지)
    setPixmap(pm.scaled(800, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setFixedSize(800, 400);
}

void MapWidget::setGraph(const FlightGraph* graph) {
    m_graph = graph;
    update();
}

void MapWidget::setSelected(const QString& code) {
    m_selected = code;
    update();
}

void MapWidget::paintEvent(QPaintEvent* event) {
    QLabel::paintEvent(event);
    if (!m_graph) return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // ── 노선 그리기 (엣지) ──
    QSet<QString> drawn;
    for (const Flight& f : m_graph->flights()) {
        QString key = (f.from < f.to) ? f.from + f.to : f.to + f.from;
        if (drawn.contains(key)) continue;
        drawn.insert(key);

        int ax = -1, ay = -1, bx = -1, by = -1;
        for (const Airport& ap : m_graph->airports()) {
            if (ap.code == f.from) { ax = ap.x; ay = ap.y; }
            if (ap.code == f.to)   { bx = ap.x; by = ap.y; }
        }
        if (ax < 0 || bx < 0) continue;

        p.setPen(QPen(QColor(255, 255, 255, 80), 1, Qt::DashLine));
        p.drawLine(ax, ay, bx, by);
    }

    // ── 공항 노드 그리기 ──
    for (const Airport& ap : m_graph->airports()) {
        bool sel = (ap.code == m_selected);

        // 선택된 노드는 노란 테두리 + 밝은 파랑
        p.setPen(QPen(sel ? QColor("#FFD700") : QColor("#FFFFFF"), sel ? 3 : 1));
        p.setBrush(sel ? QColor("#2979FF") : QColor(30, 100, 200, 200));
        p.drawEllipse(QPoint(ap.x, ap.y), RADIUS, RADIUS);

        // 공항 코드 레이블
        QFont f("Arial", 7, QFont::Bold);
        p.setFont(f);
        p.setPen(Qt::white);
        p.drawText(ap.x - 18, ap.y + RADIUS + 12, ap.code);
    }
}

void MapWidget::mousePressEvent(QMouseEvent* event) {
    if (!m_graph) return;
    QPoint click = event->pos();
    for (const Airport& ap : m_graph->airports()) {
        int dx = click.x() - ap.x;
        int dy = click.y() - ap.y;
        if (dx*dx + dy*dy <= (RADIUS+5)*(RADIUS+5)) {
            emit airportClicked(ap.code);
            return;
        }
    }
}

// ════════════════════════════════════════════
//  MainWindow
// ════════════════════════════════════════════

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("항공권 예매 시스템 [그래프 자료구조 예제]");
    buildGraph();
    setupUI();
}

void MainWindow::buildGraph() {
    // ── 공항 추가 ──
    // 좌표는 800x400 디스플레이 기준
    // 원본 이미지(640x320) 좌표에 x*(800/640), y*(400/320) 를 곱한 값
    m_graph.addAirport({"ICN", "인천",   681, 117});
    m_graph.addAirport({"GMP", "김포",   682, 117});
    m_graph.addAirport({"CJU", "제주",   681, 126});
    m_graph.addAirport({"PUS", "부산",   687, 122});
    m_graph.addAirport({"NRT", "도쿄",   712, 121});
    m_graph.addAirport({"PEK", "베이징", 659, 111});
    m_graph.addAirport({"HKG", "홍콩",   653, 150});
    m_graph.addAirport({"SIN", "싱가폴", 631, 197});
    m_graph.addAirport({"BKK", "방콕",   624, 170});
    m_graph.addAirport({"LHR", "런던",   399, 86});
    m_graph.addAirport({"PVC", "파리",   406, 91});
    m_graph.addAirport({"JFK", "뉴욕",   236, 110});
    m_graph.addAirport({"LAX", "LA",    137, 125});

    // ── 항공편 추가 (그래프 엣지) ──
    m_graph.addFlight({"LJ302", "CJU", "GMP",  85000,  "1시간 0분"});
    m_graph.addFlight({"LJ303", "GMP", "CJU",  80000,  "1시간 0분"});
    m_graph.addFlight({"LJ401", "CJU", "PUS",  75000,  "4시간 50분"});
    m_graph.addFlight({"LJ402", "PUS", "CJU",  70000,  "4시간 50분"});
    m_graph.addFlight({"KE001", "ICN", "NRT", 280000,  "2시간 30분"});
    m_graph.addFlight({"KE002", "NRT", "ICN", 275000,  "2시간 30분"});
    m_graph.addFlight({"KE005", "ICN", "LHR", 950000,  "12시간 0분"});
    m_graph.addFlight({"KE023", "ICN", "LAX", 780000,  "10시간 30분"});
    m_graph.addFlight({"OZ201", "ICN", "PEK", 220000,  "2시간 0분"});
    m_graph.addFlight({"KE631", "ICN", "HKG", 350000,  "3시간 50분"});
    m_graph.addFlight({"SQ601", "ICN", "SIN", 480000,  "6시간 30분"});
    m_graph.addFlight({"TG601", "BKK", "ICN", 430000,  "5시간 40분"});
    m_graph.addFlight({"KE085", "ICN", "JFK",1100000,  "14시간 0분"});
    m_graph.addFlight({"KE086", "JFK", "ICN",1050000,  "14시간 0분"});
}

void MainWindow::setupUI() {
    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* root = new QHBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── 왼쪽: 지도 ──
    auto* leftW = new QWidget;
    leftW->setStyleSheet("background:#0d1b2a;");
    auto* leftL = new QVBoxLayout(leftW);
    leftL->setContentsMargins(0, 0, 0, 0);
    leftL->setSpacing(0);

    m_map = new MapWidget;
    m_map->setGraph(&m_graph);
    leftL->addWidget(m_map);

    auto* bottomBar = new QLabel("공항 아이콘을 클릭하면 출발/도착 항공편이 표시됩니다");
    bottomBar->setAlignment(Qt::AlignCenter);
    bottomBar->setStyleSheet("color:#aaaaaa; padding:6px; font-size:11px;");
    leftL->addWidget(bottomBar);

    root->addWidget(leftW);

    // ── 오른쪽: 정보 패널 ──
    auto* rightW = new QWidget;
    rightW->setFixedWidth(270);
    rightW->setStyleSheet("background:#1e1e2e; border-left:1px solid #333;");
    auto* rightL = new QVBoxLayout(rightW);
    rightL->setContentsMargins(12, 14, 12, 12);
    rightL->setSpacing(6);

    m_titleLabel = new QLabel("공항을 클릭하세요");
    m_titleLabel->setStyleSheet("color:white; font-size:17px; font-weight:bold;");
    m_titleLabel->setWordWrap(true);
    rightL->addWidget(m_titleLabel);

    m_subLabel = new QLabel("좌측 노드에서 공항(볼)을 클릭하세요.");
    m_subLabel->setStyleSheet("color:#777; font-size:11px;");
    m_subLabel->setWordWrap(true);
    rightL->addWidget(m_subLabel);

    auto* sep = new QLabel;
    sep->setFixedHeight(1);
    sep->setStyleSheet("background:#444; margin:4px 0;");
    rightL->addWidget(sep);

    m_list = new QListWidget;
    m_list->setStyleSheet(R"(
        QListWidget            { background:transparent; border:none; color:white; }
        QListWidget::item      { background:#2a2a3e; border-radius:5px;
                                 margin:2px 0; padding:7px; font-size:11px; }
        QListWidget::item:hover    { background:#3a3a5e; }
        QListWidget::item:selected { background:#2979FF; }
    )");
    rightL->addWidget(m_list, 1);

    root->addWidget(rightW);

    connect(m_map,  &MapWidget::airportClicked,  this, &MainWindow::onAirportClicked);
    connect(m_list, &QListWidget::itemClicked,   this, &MainWindow::onFlightClicked);

    resize(1070, 440);
}

void MainWindow::onAirportClicked(const QString& code) {
    m_map->setSelected(code);
    refreshPanel(code);
}

void MainWindow::refreshPanel(const QString& code) {
    m_list->clear();
    QString name = m_graph.airportName(code);
    m_titleLabel->setText(QString("%1 (%2)").arg(name, code));
    m_subLabel->setText("아래 항공편을 클릭하면 예매합니다.");

    QVector<Flight> dep = m_graph.departures(code);
    QVector<Flight> arr = m_graph.arrivals(code);

    // 헤더 아이템 추가 함수 (람다)
    auto addHeader = [&](const QString& text) {
        auto* it = new QListWidgetItem(text);
        it->setForeground(QColor("#4FC3F7"));
        it->setFlags(Qt::NoItemFlags);
        QFont f; f.setBold(true); f.setPointSize(10);
        it->setFont(f);
        m_list->addItem(it);
    };

    // 출발편
    if (!dep.isEmpty()) {
        addHeader(QString("✈  출발 항공편 (%1편)").arg(dep.size()));
        for (const Flight& f : dep) {
            QString to   = m_graph.airportName(f.to);
            QString text = QString("[%1]  %2 → %3\n      ₩%4  ·  %5")
                               .arg(f.flightNo, name, to)
                               .arg(f.price).arg(f.duration);
            auto* it = new QListWidgetItem(text);
            it->setData(Qt::UserRole, QStringList{f.flightNo, f.from, f.to,
                                                  QString::number(f.price), f.duration});
            m_list->addItem(it);
        }
    }

    // 도착편
    if (!arr.isEmpty()) {
        if (!dep.isEmpty()) m_list->addItem(new QListWidgetItem(""));
        addHeader(QString("🛬  도착 항공편 (%1편)").arg(arr.size()));
        for (const Flight& f : arr) {
            QString from = m_graph.airportName(f.from);
            QString text = QString("[%1]  %2 → %3\n      ₩%4  ·  %5")
                               .arg(f.flightNo, from, name)
                               .arg(f.price).arg(f.duration);
            auto* it = new QListWidgetItem(text);
            it->setData(Qt::UserRole, QStringList{f.flightNo, f.from, f.to,
                                                  QString::number(f.price), f.duration});
            m_list->addItem(it);
        }
    }

    if (dep.isEmpty() && arr.isEmpty()) {
        auto* it = new QListWidgetItem("등록된 항공편이 없습니다.");
        it->setForeground(QColor("#888"));
        m_list->addItem(it);
    }
}

void MainWindow::onFlightClicked(QListWidgetItem* item) {
    if (!(item->flags() & Qt::ItemIsEnabled)) return;
    QStringList d = item->data(Qt::UserRole).toStringList();
    if (d.size() < 5) return;

    QString fromName = m_graph.airportName(d[1]);
    QString toName   = m_graph.airportName(d[2]);

    QMessageBox::information(this, "✔ 예매 완료",
                             QString("예매가 완료되었습니다.\n\n"
                                     "항공편: %1\n"
                                     "구간: %2 (%3) → %4 (%5)\n"
                                     "가격: ₩%6\n"
                                     "소요 시간: %7")
                                 .arg(d[0], fromName, d[1], toName, d[2], d[3], d[4]));
}