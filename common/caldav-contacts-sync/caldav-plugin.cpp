/*
    Copyright (C) 2018  Martin Klapetek <mklapetek@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "caldav-plugin.h"

#include <KJob>
#include <KConfigCore/KSharedConfig>
#include <KConfigCore/KConfigGroup>

#include <Accounts/Service>
#include <Accounts/Manager>
#include <Accounts/Account>
#include <Accounts/AccountService>
#include <QTimer>

#include "getcredentialsjob.h"
#include "core.h"

class KAccountsCalDavPlugin::Private {
public:
    Private(KAccountsCalDavPlugin *qq) { q = qq; };

    KAccountsCalDavPlugin *q;
    KSharedConfig::Ptr config;
    QTimer *syncTimer;
};


//---------------------------------------------------------------------------------------

KAccountsCalDavPlugin::KAccountsCalDavPlugin(QObject *parent)
    : KAccountsDPlugin(parent),
      d(new Private(this))
{
    d->config = KSharedConfig::openConfig(QStringLiteral("kaccounts-caldavrc"));
    d->syncTimer = new QTimer(this);
    d->syncTimer->setInterval(1000 * 60 *30);

    connect(d->syncTimer, &QTimer::timeout, this, &KAccountsCalDavPlugin::syncAllAccounts);

    syncAllAccounts();
}

KAccountsCalDavPlugin::~KAccountsCalDavPlugin()
{
}

void KAccountsCalDavPlugin::syncAllAccounts()
{
    KConfigGroup global = d->config->group("Global");
    QList<quint32> syncedAccounts = global.readEntry("syncedAccounts", QList<quint32>());

    if (syncedAccounts.isEmpty()) {
        Accounts::AccountIdList accounts = KAccounts::accountsManager()->accountListEnabled(QStringLiteral("dav-calender"));
        syncedAccounts << accounts;
    }

    qDebug() << "List of accounts to sync:" << syncedAccounts;

    Q_FOREACH (const quint32 accountId, syncedAccounts) {
        KConfigGroup currentAccount = d->config->group("account" + accountId);
        QDateTime lastSync = QDateTime::fromString(currentAccount.readEntry("lastSync", QStringLiteral("2000-09-22T00:00:00+00:00")), Qt::ISODate);
        if (QDateTime::currentDateTime() > lastSync) {
            qDebug() << "Starting caldav import for account" << accountId;
            getCredentials(accountId);
        }
    }
}

void KAccountsCalDavPlugin::onAccountCreated(const Accounts::AccountId accountId, const Accounts::ServiceList &serviceList)
{
    Accounts::Account *account = KAccounts::accountsManager()->account(accountId);

    if (!account) {
        qWarning() << "Invalid account for id" << accountId;
        return;
    }

    Q_FOREACH (const Accounts::Service &service, serviceList) {
        account->selectService(service);
        if (service.serviceType() == QLatin1String("dav-contacts") && account->isEnabled()) {
            qDebug() << "Starting caldav import for account" << accountId << "and service" << service.serviceType();
            getCredentials(accountId);
        }
    }
}

void KAccountsCalDavPlugin::onAccountRemoved(const Accounts::AccountId accountId)
{
}

void KAccountsCalDavPlugin::onServiceEnabled(const Accounts::AccountId accountId, const Accounts::Service &service)
{
}

void KAccountsCalDavPlugin::onServiceDisabled(const Accounts::AccountId accountId, const Accounts::Service &service)
{
}

void KAccountsCalDavPlugin::getCredentials(const Accounts::AccountId accountId)
{
    GetCredentialsJob *credentialsJob = new GetCredentialsJob(accountId, this);
    connect(credentialsJob, &GetCredentialsJob::finished, this, &KAccountsCalDavPlugin::importCalender);
    credentialsJob->start();
}

void KAccountsCalDavPlugin::importCalender(KJob *job)
{
    GetCredentialsJob *credentialsJob = qobject_cast<GetCredentialsJob*>(job);
    job->deleteLater();

    if (!credentialsJob) {
        return;
    }

    const QVariantMap &data = credentialsJob->credentialsData();
    Accounts::Account *account = KAccounts::accountsManager()->account(credentialsJob->accountId());

    if (!account) {
        return;
    }

    QUrl carddavUrl = account->value("carddavUrl").toUrl();

    qDebug() << "Using: host:" << carddavUrl.host() << "path:" << carddavUrl.path();

    const QString &userName = data.value("AccountUsername").toString();

}
