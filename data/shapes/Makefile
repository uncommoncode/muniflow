california-latest.osm.pbf:
	curl -o california-latest.osm.pbf "http://download.geofabrik.de/north-america/us/california-latest.osm.pbf"

sf-extracted.xml:
	osmosis enableDateParsing=no file=california-latest.osm.pbf --bounding-box top=37.833 left=-122.538 bottom=37.7026 right=-122.3534
