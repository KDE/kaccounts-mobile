/*
    Copyright (C) 2015  Martin Klapetek <mklapetek@kde.org>

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

#ifndef KACCOUNTSCARDDAVPLUGIN_H
#define KACCOUNTSCARDDAVPLUGIN_H

#include <kaccountsdplugin.h>

namespace Accounts {
    class Manager;
}

class KJob;

class KAccountsCardDavPlugin : public KAccountsDPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kaccounts.DPlugin")
    Q_INTERFACES(KAccountsDPlugin)

public:
    KAccountsCardDavPlugin(QObject *parent = nullptr);
    ~KAccountsCardDavPlugin();

public Q_SLOTS:
    void onAccountCreated(const Accounts::AccountId accountId, const Accounts::ServiceList &serviceList);
    void onAccountRemoved(const Accounts::AccountId accountId);
    void onServiceEnabled(const Accounts::AccountId accountId, const Accounts::Service &service);
    void onServiceDisabled(const Accounts::AccountId accountId, const Accounts::Service &service);

private Q_SLOTS:
    void importContacts(KJob *job);
    void getCredentials(const Accounts::AccountId accountId);
    void syncAllAccounts();

private:
    class Private;
    Private * const d;
};

#endif // KACCOUNTSCARDDAVPLUGIN_H
