const PLC_URI = 'http://localhost:8081/wind-speed'
const WEATHER_URI_GET_REQUEST = 'http://localhost:8081/getWeather'
const WEATHER_URI_STATIONS_REQUEST = 'http://localhost:8081/getStations'
var map;


$(function () {
    loadMap();

    $('#dateto').prop("max", getDateFormatted(getDayBefore(getDayBefore(new Date()))));
    $('#datefrom').prop("max", getDateFormatted(getDayBefore(new Date())));

    if (!$('#dateto').val()) {
        $('#dateto').val(getDateFormatted(getDayBefore(new Date())));
    }
    if (!$('#datefrom').val()) {
        $('#datefrom').val(getDateFormatted(getDayBefore(getDayBefore(new Date()))));
    }
    $('#datefrom').change(function () {
        let to = Date.parse($('#dateto').val())
        let from = Date.parse($('#datefrom').val())
        if (!isNaN(from) && (isNaN(to) || from >= to)) {
            $('#dateto').val($('#datefrom').val());
        }
    });
    $('#dateto').change(function () {
        let to = Date.parse($('#dateto').val())
        let from = Date.parse($('#datefrom').val())
        if (!isNaN(to) && (isNaN(from) || from < to)) {
            $('#datefrom').val(getDayBefore(to)).prop("max", getDayBefore(to));
        }
    });
    $('#start').click(function () {
        let date = {}
        date.from = $('#datefrom').val()
        date.to = $('#dateto').val()
        date.interval = $('#interval').val() * 1000
        let station = {}
        station.name = $('#stationname_selected').val()
        station.id = $('#stationid_selected').val()

        console.log(station.id, station.name, date.from, date.to, date.interval)
        $("#start").prop("disabled", true)
        $('#mapdiv').fadeOut('slow');
        getWeather(station.id, date.from, date.to, date.interval)
    });

});

function loadMap() {
    $.getJSON(WEATHER_URI_STATIONS_REQUEST).done(
        function (data) {
            console.log('loaded', data.features)
            map = L.map('map', {
                center: [52.5162731, 13.367294],
                minZoom: 3,
                zoom: 10,
                layers: [
                    L.tileLayer('http://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}.png', {
                        attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors, &copy; <a href="http://cartodb.com/attributions">CartoDB</a>'
                    }),
                    L.geoJson(data, {
                        onEachFeature: function (feature, layer) {
                            layer.on('click', layerClickHandler);
                        }
                    })
                ]
            })
        }
    );
}

function layerClickHandler(e) {
    let station = e.target.feature.properties;
    $('#stationid_selected').val(station.id);
    $('#stationname_selected').val(station.name);
    $('#start').prop("disabled", false)
    console.log(station.name, $('#stationname_selected').val(), station.id, $('#stationid_selected').val())
}

function getWeather(station_id, from, to, interval) {
    let index = 0;
    console.log("form input", station_id, from, interval);
    $('#alert').prop('class', "alert alert-secondary").html('Request sent!');
    $.getJSON(
        WEATHER_URI_GET_REQUEST,
        {station_id: station_id, datefrom: from, dateto: to},
        function (data) {
            if (data.wind.length === 0) {
                $('#alert').prop('class', "alert alert-secondary").html('Weather station ' + data.station_id + ' has no wind recordings !');
            } else {
                $('#alert').prop('class', "alert alert-success").html('Response of ' + data.wind.length + ' entrie(s) received!');
            }
            populateTable(data);
            updateProgressBar(index, data.wind.length);
            let next = getNextValue(index);
            (function handleNextWindFix() {
                if (next === null || index >= data.wind.length) {
                    handleFinish();
                }
                $.ajax({
                    url: PLC_URI,
                    contentType: 'application/json',
                    type: 'PUT',
                    data: JSON.stringify({"windSpeed": next}),
                    dataType: "text",
                    complete: function (xhr, textStatus) {
                        if (xhr.status > 201) {
                            handleError(xhr, textStatus);
                        } else {
                            if (index < getTableLength()) {
                                next = updateTable(index, xhr, textStatus);
                                index += 1;
                                // Schedule the next request when the current one's complete
                                setTimeout(handleNextWindFix, getIntervalInSec());
                            } else {
                                handleFinish();
                            }
                        }
                    }
                });
            })();
        }
    );
}

function getIntervalInSec() {
    return $('#interval').val() * 1000
}

function populateTable(data) {
    console.log("recieved", data.wind.length, "added to table", $("#servermsg > tr").length);
    $("#servermsg tr").remove();
    $.each(data.wind, function (index, element) {
        $('#servermsg').append('<tr><td>' + element.wind_speed + '</td><td>' + element.ts + '</td><td></td></tr>')
    });
    if (typeof $('#servermsg').find('tr').length !== 'undefined') {
        lengthinitial = $('#servermsg > tr').length;
    } else {
        lengthinitial = 0;
    }
    return lengthinitial;
}

function updateTable(index, xhr, textStatus) {
    console.log('update table');
    $("#servermsg").find("tr").eq(index).find("td").eq(2).html(xhr.status + " (" + textStatus + ")");
    updateProgressBar();
    return getNextValue(index + 1);
}

function updateProgressBar(index) {
    console.log('update progress bar');
    tbllength = getTableLength()
    if (tbllength > 0) {
        var lengthfract = (index / tbllength) * 100;
        $('.progress-bar').prop("style", "width: " + lengthfract + "%;").prop("aria-valuenow", lengthfract);
    } else {
        $('.progress-bar').prop("style", "width: " + lengthfract + "%;").prop("aria-valuenow", 100);

    }
}

function handleFinish() {
    $.ajax({
        url: PLC_URI,
        contentType: 'application/json',
        type: 'PUT',
        data: JSON.stringify({'windSpeed': 0.0}),
        dataType: "text",
        complete: function (xhr, textStatus) {
            if (xhr.status > 200) {
                handleError(xhr, textStatus);
            }
        }
    });
    $('#alert').val('Simulation finished! All entrie(s) sent!');
    $('#servermsg').find('tr').fadeOut();
    $('#mapdiv').fadeIn();
    $('#start').prop('disabled', false);
}

function handleError(xhr, textStatus) {
    console.error(xhr.status, textStatus);
    $('#alert').prop('class', "alert alert-danger").html('Error (' + xhr.status + ')! ' + textStatus);
    $('#mapdiv').fadeIn();
    $('#start').prop('disabled', false);
}

function getTableLength() {
    return $('#servermsg').find('tr').length;
}

function getNextValue(index) {
    return parseFloat($("#servermsg").find("tr").eq(index).find("td").eq(0).text());
}

function getDayBefore(date) {
    let now = new Date(date);
    return new Date(date.getTime() - 86400000);
}

function getDateFormatted(date) {
    var day = ("0" + date.getDate()).slice(-2);
    var month = ("0" + (date.getMonth() + 1)).slice(-2);
    return date.getFullYear() + "-" + (month) + "-" + (day);
}
