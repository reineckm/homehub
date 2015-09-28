# HomeHub #
HomeHub ist eine Heimautomatisierung mit Arduino und ESP8266 Modulen.
Die Idee ist es, das sich verschiedene steuerbare Geräte im WLAN anmelden und eine Datenstruktur auf einer zentralen Datenbank hinterlegen. Diese Datenstruktur enthält mindestens die folgenden Informationen:

- ip = IP-Adresse im lokalen WLAN
- name = Der Name des Geräts. Dieser darf nur einmal in einem Netz vorkommen.

Diese Datenstruktur wird auf einer parse.com Datenbank abgelegt und von der  Webapp in Unterordner App gelesen.

## Zugänge ##
Diese Anwendung benötigt 2 Zugänge die natürlich nicht auf Github liegen - einmal für das WLAN und für parse.com. Die Zugänge müssen in 

	SenseoServer/Credentials.h und
	App/Credentials.js

eingetragen werden.

## Senseo ##
Erstes verknüpftes Gerät ist meine Senseo Kaffeemaschine. die Hardware besteht aus:

- 2 Servos, eines bedient den Power Knopf der Maschine, das andere bedient den Taster für eine große Tasse.
- 1 ESP8266 Modul, wählt sich im WLAN ein und stellt eine einfache Webseite zur verfügung die als Userinterface dient. Drückt der Nutzer den einzigen Button auf der Seite, wird auf die Seite /kaffee geleitet. Wenn dies passiert, wird der Kaffee gebrüht
- 1 Digispark Arduino Modul, dieses kommuniziert über einen Pin mit dem ESP8266. Wird auf dem IN_PIN (3) für eine Sekunde High Pegel gemessen, beginnt das Modul damit, mit den Servos die Senseo Maschine einzuschalten und Kaffee zu brühen.
- Spannungswandler (3,3V) und Pulldown Wiederstände

Die Programme für die Hardware sind in KaffeServer und Senseo abgelegt. JS, CSS und ein Bild für das Userinterface liegen nicht auf dem ESP8266 sondern werden von einem Webspace geladen.
