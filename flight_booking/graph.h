#pragma once
#include <QString>
#include <QVector>
#include <QMap>

// 항공편 (그래프의 엣지)
struct Flight {
    QString flightNo;
    QString from;
    QString to;
    int     price;
    QString duration;
};

// 공항 (그래프의 노드)
struct Airport {
    QString code;
    QString name;
    int x, y;   // worldmap.jpg (640x320 기준) 위 픽셀 좌표
};

// 그래프 클래스 (인접 리스트 방식)
class FlightGraph {
public:
    void addAirport(const Airport& ap) {
        m_airports.append(ap);
        m_adj[ap.code] = QVector<Flight>();
    }

    void addFlight(const Flight& f) {
        m_flights.append(f);
        m_adj[f.from].append(f);
    }

    const QVector<Airport>& airports() const { return m_airports; }
    const QVector<Flight>&  flights()  const { return m_flights;  }

    // 특정 공항에서 출발하는 항공편
    QVector<Flight> departures(const QString& code) const {
        return m_adj.value(code);
    }

    // 특정 공항에 도착하는 항공편
    QVector<Flight> arrivals(const QString& code) const {
        QVector<Flight> result;
        for (const Flight& f : m_flights)
            if (f.to == code) result.append(f);
        return result;
    }

    QString airportName(const QString& code) const {
        for (const Airport& ap : m_airports)
            if (ap.code == code) return ap.name;
        return code;
    }

private:
    QVector<Airport>          m_airports;
    QVector<Flight>           m_flights;
    QMap<QString, QVector<Flight>> m_adj;  // 인접 리스트
};