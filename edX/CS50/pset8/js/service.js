/**
 * service.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Implements a shuttle service.
 */

// default height
var HEIGHT = 0.8;

// default latitude
var LATITUDE = 42.3745615030193;

// default longitude
var LONGITUDE = -71.11803936751632;

// default heading
var HEADING = 1.757197490907891;

// default number of seats
var SEATS = 10;

// default velocity
var VELOCITY = 50;

// global reference to shuttle's marker on 2D map
var bus = null;

// global reference to 3D Earth
var earth = null;

// global reference to 2D map
var map = null;

// global reference to shuttle
var shuttle = null;

// global points
var points = 0;

//TODO
var COLOR =
{
    "Adams House": {color: "#DF01A5"},
    "Cabot House": {color: "#8A0886"},
    "Currier House": {color: "#0B3861"},
    "Dunster House": {color: "#5F4C0B"},
    "Eliot House": {color: "#088A29"},
    "Kirkland House": {color: "#81F7D8"},
    "Leverett House": {color: "##141907"},
    "Lowell House": {color: "#157F6F"},
    "Mather House": {color: "#111111"},
    "Pforzheimer House": {color: "#953ABC"},
    "Quincy House": {color: "#5B650F"},
    "Winthrop House": {color: "#DC270B"}
};

// load version 1 of the Google Earth API
google.load("earth", "1");

// load version 3 of the Google Maps API
google.load("maps", "3", {other_params: "sensor=false"});

// once the window has loaded
$(window).load(function() {

    // listen for keydown anywhere in body
    $(document.body).keydown(function(event) {
        return keystroke(event, true);
    });

    // listen for keyup anywhere in body
    $(document.body).keyup(function(event) {
        return keystroke(event, false);
    });

    // listen for click on Drop Off button
    $("#dropoff").click(function(event) {
        dropoff();
    });

    // listen for click on Pick Up button
    $("#pickup").click(function(event) {
        pickup();
    });

    // load application
    load();
});

// unload application
$(window).unload(function() {
    unload();
});

/**
 * Renders seating chart.
 */
function chart()
{
    var html = "<ol start='0'>";
    for (var i = 0; i < shuttle.seats.length; i++)
    {
        if (shuttle.seats[i] == null)
        {
            html += "<li>Empty Seat</li>";
        }
        else
        {
            //TODO
            html += "<li style=color:" + COLOR[shuttle.seats[i].house].color + "><b>" + shuttle.seats[i].name + ", " + shuttle.seats[i].house + "</b></li>";
        }
    }
    html += "</ol>";
    $("#chart").html(html);
}

/**
 * Drops up passengers if their stop is nearby.
 */
function dropoff()
{
    var flag = 0;
    for (var k = 0; k < shuttle.seats.length; k++)
    {
        if(shuttle.seats[k] != null)
        {
            lat = HOUSES[shuttle.seats[k].house].lat;
            long = HOUSES[shuttle.seats[k].house].lng;
            var dis = shuttle.distance(lat, long);
            if (dis < 30)
            {
                shuttle.seats[k] = null;
                points++;
                $("#announcements").html("Points: " + points);
                chart();
                flag = 1;
            }
        }
    }
    if(flag == 0)
    {
        $("#announcements").html("No houses are close enough for drop off.");
    }
}

/**
 * Called if Google Earth fails to load.
 */
function failureCB(errorCode) 
{
    // report error unless plugin simply isn't installed
    if (errorCode != ERR_CREATE_PLUGIN)
    {
        alert(errorCode);
    }
}

/**
 * Handler for Earth's frameend event.
 */
function frameend() 
{
    shuttle.update();
}

/**
 * Called once Google Earth has loaded.
 */
function initCB(instance) 
{
    // retain reference to GEPlugin instance
    earth = instance;

    // specify the speed at which the camera moves
    earth.getOptions().setFlyToSpeed(100);

    // show buildings
    earth.getLayerRoot().enableLayerById(earth.LAYER_BUILDINGS, true);

    // disable terrain (so that Earth is flat)
    earth.getLayerRoot().enableLayerById(earth.LAYER_TERRAIN, false);

    // prevent mouse navigation in the plugin
    earth.getOptions().setMouseNavigationEnabled(false);

    // instantiate shuttle
    shuttle = new Shuttle({
        heading: HEADING,
        height: HEIGHT,
        latitude: LATITUDE,
        longitude: LONGITUDE,
        planet: earth,
        seats: SEATS,
        velocity: VELOCITY
    });

    // synchronize camera with Earth
    google.earth.addEventListener(earth, "frameend", frameend);

    // synchronize map with Earth
    google.earth.addEventListener(earth.getView(), "viewchange", viewchange);

    // update shuttle's camera
    shuttle.updateCamera();

    // show Earth
    earth.getWindow().setVisibility(true);

    // render seating chart
    chart();

    // populate Earth with passengers and houses
    populate();
}

/**
 * Handles keystrokes.
 */
function keystroke(event, state)
{
    // ensure we have event
    if (!event)
    {
        event = window.event;
    }

    // left arrow
    if (event.keyCode == 37)
    {
        shuttle.states.turningLeftward = state;
        //TODO
        $("#announcements").html("No Announcements");
        return false;
    }

    // up arrow
    else if (event.keyCode == 38)
    {
        shuttle.states.tiltingUpward = state;
        //TODO
        $("#announcements").html("No Announcements");
        return false;
    }

    // right arrow
    else if (event.keyCode == 39)
    {
        shuttle.states.turningRightward = state;
        //TODO
        $("#announcements").html("No Announcements");
        return false;
    }

    // down arrow
    else if (event.keyCode == 40)
    {
        shuttle.states.tiltingDownward = state;
        //TODO
        $("#announcements").html("No Announcements");
        return false;
    }

    // A, a
    else if (event.keyCode == 65 || event.keyCode == 97)
    {
        shuttle.states.slidingLeftward = state;
        //TODO
        $("#announcements").html("No Announcements");
        return false;
    }

    // D, d
    else if (event.keyCode == 68 || event.keyCode == 100)
    {
        shuttle.states.slidingRightward = state;
        //TODO
        $("#announcements").html("No Announcements");
        return false;
    }
  
    // S, s
    else if (event.keyCode == 83 || event.keyCode == 115)
    {
        shuttle.states.movingBackward = state;
        //TODO
        $("#announcements").html("No Announcements");
        return false;
    }

    // W, w
    else if (event.keyCode == 87 || event.keyCode == 119)
    {
        shuttle.states.movingForward = state;
        //TODO
        $("#announcements").html("No Announcements");
        return false;
    }
    // speed up with 1
    else if (event.keyCode == 49)
    {
        shuttle.velocity = shuttle.velocity + 2;
        $("#announcements").html("Current speed = " + shuttle.velocity);
        return false;
    }

    // speed down with 0
    else if (event.keyCode == 48)
    {
        shuttle.velocity = shuttle.velocity - 2;
        $("#announcements").html("Current speed = " + shuttle.velocity);
        return false;
    }

    return true;
}

/**
 * Loads application.
 */
function load()
{
    // embed 2D map in DOM
    var latlng = new google.maps.LatLng(LATITUDE, LONGITUDE);
    map = new google.maps.Map($("#map").get(0), {
        center: latlng,
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        scrollwheel: false,
        zoom: 17,
        zoomControl: true
    });

    // prepare shuttle's icon for map
    bus = new google.maps.Marker({
        icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/bus.png",
        map: map,
        title: "you are here"
    });

    // embed 3D Earth in DOM
    google.earth.createInstance("earth", initCB, failureCB);
}

/**
 * Picks up nearby passengers.
 */
function pickup()
{
    var check = 0;
    for (var i in PASSENGERS)
    {
        if(PASSENGERS[i].placemark != null)
        {
            lat = PASSENGERS[i].placemark.getGeometry().getLatitude();
            long = PASSENGERS[i].placemark.getGeometry().getLongitude();
            var dis = shuttle.distance(lat, long);
            if (dis < 15 )
            {
                check = 1;
                availableSeat = 0;
                if (PASSENGERS[i].house in HOUSES == false)
                {
                    $("#announcements").html("Sorry no freshman allowed.");
                    break;
                }
                for (var j = 0; j < shuttle.seats.length; j++)
                {
                    if (shuttle.seats[j] == null)
                    {
                        availableSeat = 1;
                        shuttle.seats[j] = PASSENGERS[i];
                        $("#announcements").html("Points: " + points);
                        chart();
                        var features = earth.getFeatures();
                        features.removeChild(PASSENGERS[i].placemark);
                        PASSENGERS[i].placemark = null;
                        PASSENGERS[i].marker.setMap(null);
                        PASSENGERS[i].marker = null;
                        break;
                    }
                }
                if (availableSeat == 0)
                {
                    $("#announcements").html("Sorry, No seats available!");
                }
            }
        }
    }
    if (check == 0)
    {
        $("#announcements").html("No people close enough for pickup!");
    }
}

/**
 * Populates Earth with passengers and houses.
 */
function populate()
{
    // mark houses
    for (var house in HOUSES)
    {
        // plant house on map
        new google.maps.Marker({
            icon: "https://google-maps-icons.googlecode.com/files/home.png",
            map: map,
            position: new google.maps.LatLng(HOUSES[house].lat, HOUSES[house].lng),
            title: house
        });
    }

    // get current URL, sans any filename
    var url = window.location.href.substring(0, (window.location.href.lastIndexOf("/")) + 1);

    // scatter passengers
    for (var i = 0; i < PASSENGERS.length; i++)
    {
        // pick a random building
        var building = BUILDINGS[Math.floor(Math.random() * BUILDINGS.length)];

        // prepare placemark
        var placemark = earth.createPlacemark("");
        placemark.setName(PASSENGERS[i].name + " to " + PASSENGERS[i].house);

        // prepare icon
        var icon = earth.createIcon("");
        icon.setHref(url + "/img/" + PASSENGERS[i].username + ".jpg");

        // prepare style
        var style = earth.createStyle("");
        style.getIconStyle().setIcon(icon);
        style.getIconStyle().setScale(4.0);

        // prepare stylemap
        var styleMap = earth.createStyleMap("");
        styleMap.setNormalStyle(style);
        styleMap.setHighlightStyle(style);

        // associate stylemap with placemark
        placemark.setStyleSelector(styleMap);

        // prepare point
        var point = earth.createPoint("");
        point.setAltitudeMode(earth.ALTITUDE_RELATIVE_TO_GROUND);
        point.setLatitude(building.lat);
        point.setLongitude(building.lng);
        point.setAltitude(0.0);

        // associate placemark with point
        placemark.setGeometry(point);

        // add placemark to Earth
        earth.getFeatures().appendChild(placemark);

        // add marker to map
        var marker = new google.maps.Marker({
            icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/man.png",
            map: map,
            position: new google.maps.LatLng(building.lat, building.lng),
            title: PASSENGERS[i].name + " at " + building.name
        });

        // TODO: remember passenger's placemark and marker for pick-up's sake
        PASSENGERS[i].marker = marker;
        PASSENGERS[i].placemark = placemark;
    }
}

/**
 * Handler for Earth's viewchange event.
 */
function viewchange() 
{
    // keep map centered on shuttle's marker
    var latlng = new google.maps.LatLng(shuttle.position.latitude, shuttle.position.longitude);
    map.setCenter(latlng);
    bus.setPosition(latlng);
}

/**
 * Unloads Earth.
 */
function unload()
{
    google.earth.removeEventListener(earth.getView(), "viewchange", viewchange);
    google.earth.removeEventListener(earth, "frameend", frameend);
}
