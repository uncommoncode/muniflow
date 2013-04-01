#include "PassengerData.hpp"

#include <QtCore>
#include <QDateTime>
#include <QDate>
#include <QTime>

void readPassengerData(const QString &filePath, QVector<PassengerEntry> *entries) {
    QFile file(filePath);

    Q_ASSERT(file.open(QFile::ReadOnly | QIODevice::Text));

    // skip first line
    QTextStream stream(&file);
    stream.readLine();

    while (!stream.atEnd()) {
      PassengerEntry entry;
      QString line = stream.readLine();
      // split on comma
      QStringList fields = line.split(",");
      Q_ASSERT(fields.size() == 22);

      entry.stopSequence = fields.at(0).toULong();
      entry.stopId = fields.at(1).toULong();
      entry.stopName = fields.at(2);
      entry.peopleOn = fields.at(3).toULong();
      entry.peopleOff = fields.at(4).toULong();
      entry.load = fields.at(5).toULong();

      QDate date;
      // MO,DAY,YR
      int month = fields.at(6).toInt();
      int day = fields.at(7).toInt();
      int year = fields.at(8).toInt();
      date.setDate(year, month, day);

      entry.route = fields.at(9);
      entry.location = toGeoLocation(fields.at(11), fields.at(10));
      entry.location.longitude = - entry.location.longitude;
      entry.tripId = fields.at(12).toULong();
      entry.vehicleNumber = fields.at(14).toULong();

      // DIR,VEHNO,TIMESTOP,TIMEDOORCLOSE,TIMEPULLOUT
      QTime time;
      time.fromString(fields.at(17));

      QDateTime dateTime;
      dateTime.setDate(date);
      dateTime.setTime(time);

      entry.time = toTime(dateTime);

      entries->push_back(entry);
    }

    file.close();
}
