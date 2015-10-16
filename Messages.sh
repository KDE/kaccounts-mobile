#! /usr/bin/env bash

find . -name "*.provider.in" -o -name "*.service.in" -type f | xargs -n1 intltool-extract -l -type="gettext/xml"
$XGETTEXT `find . -name "*.h" -o -name "*.cpp"` -kN_ -c -o $podir/kaccounts-mobile.pot
rm -rf ./tmp/
