from dwdweather import DwdWeather
from datetime import date, datetime, timedelta
import json


def getWeather(req_station_id, req_from, req_until):
    datefrom = lastts(req_from)
    dateto = lastts(req_until)
    wind = weather(station_id=int(req_station_id), datefrom=datefrom, dateto=dateto)
    return json.dumps({'station_id': req_station_id, 'wind': wind}, sort_keys=True, indent=4)


def weather(station_id, datefrom=datetime.now(), dateto=datetime.now()):
    if datefrom > dateto:
        cache = datefrom
        datefrom = dateto
        dateto = cache

    result = []
    record = query(station_id, datefrom)
    if record != None:
        result.append(
            {'ts': datefrom.strftime("%Y-%m-%d %H:%M:%S"),
             'wind_speed': record['wind_speed']}
        )
    else:
        return result
    datefrom = datefrom + timedelta(hours=1)
    while datefrom <= dateto:
        record = query(station_id, datefrom)
        if record != None:
            result.append(
                {'ts': datefrom.strftime("%Y-%m-%d %H:%M:%S"),
                 'wind_speed': record['wind_speed']}
            )
        datefrom = datefrom + timedelta(hours=1)
    return result


def query(station, datenow):
    dwd = DwdWeather(resolution="hourly", category_names=["wind"])
    return dwd.query(station_id=station, timestamp=datenow)


def lastts(input):
    """
    returns a date object set to yesterday at 23:00 o'clock if the given date is in the future
    """
    check = datetime.combine(date.fromisoformat(input), datetime.min.time())
    latest = datetime.combine(datetime.today(), datetime.min.time())
    if latest < check:
        return latest - timedelta(hours=1)
    else:
        return check
