california.osm.bz2:
	curl -o california.osm.bz2 "http://download.geofabrik.de/north-america/us/california-130330.osm.bz2"

sf-extracted.xml: california.osm.bz2
	bzcat california.osm.bz2 | osmosis --read-xml enableDateParsing=no file=- --bounding-box top=37.833 left=-122.538 bottom=37.7026 right=-122.3534 --write-xml file=sf-extracted.xml

sf-roads.xml: sf-extracted.xml
	osmosis --read-xml file=sf-extracted.xml --way-key keyList=highway --tag-filter reject-nodes --tag-filter reject-relations --write-xml file=sf-roads.xml 

all: sf-extracted.xml sf-roads.xml

