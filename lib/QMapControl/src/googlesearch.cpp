/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2007 - 2008 Kai Winter
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with QMapControl. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: kaiwinter@gmx.de
* Program URL   : http://qmapcontrol.sourceforge.net/
*
*/

#include "googlesearch.h"
#include <QWaitCondition>
#include <QUrl>
namespace qmapcontrol
{
    GoogleSearch::GoogleSearch(QObject* parent)
        :QObject(parent), http(new QHttp(this))
    {
        connect(http, SIGNAL(requestFinished(int, bool)),
                this, SLOT(requestFinished(int, bool)));
    }

    GoogleSearch::~GoogleSearch()
    {
        http->clearPendingRequests();
        delete http;
    }

    void GoogleSearch::setProxy(QString host, int port)
    {
        http->setProxy(host, port);
    }

    void GoogleSearch::searchPlace(const QString& place)
    {
        QString host("maps.google.com");
        QString url = host + QString("/maps/geo?output=xml&q=%1").arg(QString(QUrl::toPercentEncoding(place)));

        http->setHost(host);
        QHttpRequestHeader header("GET", url);
        header.setValue("User-Agent", "Mozilla");
        header.setValue("Host", host);

        int getId = http->request(header);

        if (vectorMutex.tryLock())
        {
            loading[getId] = url;
            vectorMutex.unlock();
        }
    }

    void GoogleSearch::requestFinished(int id, bool error)
    {
        if (error)
        {
            if (vectorMutex.tryLock())
            {
                if (loading.contains(id))
                    loading.remove(id);

                vectorMutex.unlock();
            }

            emit searchFinished(0.0, 0.0, true);
        }
        else if (vectorMutex.tryLock())
        {
            if (loading.contains(id))
            {
                loading.remove(id);
                vectorMutex.unlock();

                if (http->bytesAvailable() > 0)
                    parseXML(http->readAll());
                else
                    emit searchFinished(0.0, 0.0, true);
            }
            else
                vectorMutex.unlock();
        }
    }

    void GoogleSearch::abortLoading()
    {
        http->clearPendingRequests();
    }

    void GoogleSearch::parseXML(const QByteArray &data)
    {
        double lon = 0.0, lat = 0.0;

        //qDebug() << QString(data);
        emit searchFinished(lon, lat, false);
    }
}
