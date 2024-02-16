[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver LAN8720

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/lan8720/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der LAN8720A/LAN8720Ai ist ein stromsparender 10BASE-T/100BASE-TX Physical Layer (PHY)-Transceiver mit variabler E/A-Spannung, der mit den Standards IEEE 802.3-2005 kompatibel ist. Der LAN8720A/LAN8720Ai unterstützt die Kommunikation mit einem Ethernet-MAC über eine Standard-RMII-Schnittstelle. Es enthält einen Vollduplex-10-BASE-T/100BASE-TX-Transceiver und unterstützt den Betrieb mit 10 Mbit/s (10BASE-T) und 100 Mbit/s (100BASE-TX). Der LAN8720A/LAN8720Ai implementiert Auto-Negotiation, um automatisch die bestmögliche Geschwindigkeit und den bestmöglichen Duplex-Betriebsmodus zu ermitteln. Die HP Auto-MDIX-Unterstützung ermöglicht die Verwendung von Direktverbindungs- oder Crossover-LAN-Kabeln.

LibDriver LAN8720 ist der von LibDriver gestartete Vollfunktionstreiber von LAN8720. LibDriver LAN8720 bietet grundlegende Konfiguration, automatische Aushandlung und andere Funktionen. LibDriver ist MISRA-kompatibel.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver LAN8720-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver LAN8720 SMI, RMII.

/test enthält den Testcode des LibDriver LAN8720-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver LAN8720-Beispielcode.

/doc enthält das LibDriver LAN8720-Offlinedokument.

/Datenblatt enthält LAN8720-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige SMI, RMII-Schnittstellenvorlage und stellen Sie Ihren Plattform-SMI, RMII-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

#### example basic

```C
#include "driver_lan8720_basic.h"

uint8_t res;
lan8720_speed_indication_t speed_indication;

/* basic init */
res = lan8720_basic_init(0x01);
if (res != 0)
{
    return 1;
}

...
    
/* check auto negotiation */
if (lan8720_basic_auto_negotiation(&speed_indication) != 0)
{
    (void)lan8720_basic_deinit();
    
    return 1;
}

...

/* basic deinit */    
(void)lan8720_basic_deinit();

return 0;
```
### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/lan8720/index.html](https://www.libdriver.com/docs/lan8720/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.