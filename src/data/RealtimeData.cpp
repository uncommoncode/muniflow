#include "RealtimeData.hpp"

#include <QtCore>

void readRealtimeData(const QString& filePath, QVector<RealtimeEntry> *entries) {
    QFile file(filePath);

    Q_ASSERT(file.open(QFile::ReadOnly | QIODevice::Text));

    // skip first line
    QTextStream stream(&file);
    stream.readLine();

    while (!stream.atEnd()) {
      RealtimeEntry entry;
      QString line = stream.readLine();
      // split on comma
      QStringList fields = line.split(",");
      Q_ASSERT(fields.size() == 10);
      entry.name = fields.at(0);
      entry.vehicleAssignment = fields.at(1);
      entry.tripId = fields.at(2).toULong();
      entry.blockName = fields.at(3);
      entry.doubleCar = (fields.at(4) == "N") ? false : true;
      entry.carId = fields.at(5).toULong();
      entry.stopId = fields.at(6).toULong();
      entry.position = toGeoLocation(fields.at(7), fields.at(8));
      entry.arrivalTime = QDateTime::fromString(fields.at(9), "M/d/yyyy h:mm:ss.000000 AP");

      entries->push_back(entry);
    }

    file.close();
}
