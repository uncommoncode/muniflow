SOURCES += src/main.cpp \
	src/data/RealtimeData.cpp src/data/PassengerData.cpp src/data/ScheduleData.cpp src/data/VisData.cpp \
	src/data/GeoLocation.cpp src/data/Time.cpp \
	src/vis/Plot.cpp src/vis/TimeController.cpp src/vis/VisWindow.cpp \
	src/vis-proto/Simple.cpp \
	src/vis-proto/Particle.cpp \
	src/vis-proto/DLCycle.cpp
CONFIG += qt debug
INCLUDEPATH += src/
