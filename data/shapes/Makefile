california.osm.bz2:
	curl -o california.osm.bz2 "http://download.geofabrik.de/north-america/us/california-130330.osm.bz2"

sf-extracted.xml: california.osm.bz2
	bzcat california.osm.bz2 | osmosis --read-xml enableDateParsing=no file=- --bounding-box top=37.833 left=-122.538 bottom=37.7026 right=-122.3534 --write-xml file=sf-extracted.xml

sf-roads.xml: sf-extracted.xml
	osmosis --read-xml file=sf-extracted.xml --way-key keyList=highway --tag-filter reject-nodes --tag-filter reject-relations --write-xml file=sf-roads.xml 

sf-toner-background.png:
	curl -o sf-toner-background.png "https://s3.amazonaws.com/images.m2i.stamen.com/toner-background_QXhLwyx9-38.png"

sf-watercolor.jpeg:
	curl -o sf-watercolor.jpeg "https://s3.amazonaws.com/images.m2i.stamen.com/watercolor_eGxWVQAJToc.jpeg"

sf-terrain-lines.png:
	curl -o sf-terrain-lines.png "https://s3.amazonaws.com/images.m2i.stamen.com/terrain-lines_RjYywa_5I6w.png"

sf-stamen-images: sf-toner-background.png sf-terrain-lines.png sf-watercolor.jpeg

all: sf-extracted.xml sf-roads.xml sf-stamen-images

