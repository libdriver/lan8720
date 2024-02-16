[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver LAN8720

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/lan8720/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

LAN8720A/LAN8720Ai は、IEEE 802.3-2005 標準に準拠した可変 I/O 電圧を備えた低電力 10BASE-T/100BASE-TX 物理層 (PHY) トランシーバーです。LAN8720A/LAN8720Ai は、イーサネット MAC との通信をサポートします。 標準のRMIIインターフェイス。 全二重 10-BASE-T/100BASE-TX トランシーバーを内蔵しており、10Mbps (10BASE-T) および 100Mbps (100BASE-TX) の動作をサポートします。 LAN8720A/LAN8720Ai はオートネゴシエーションを実装し、可能な限り最適な速度と二重動作モードを自動的に決定します。 HP Auto-MDIX サポートにより、直接接続またはクロスオーバー LAN ケーブルの使用が可能になります。

LibDriver LAN8720 は、LibDriver によって起動される LAN8720 の全機能ドライバーです。LibDriver LAN8720 は、基本構成、自動ネゴシエーションおよびその他の機能を提供します。 LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver LAN8720のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver LAN8720用のプラットフォームに依存しないSMI, RMIIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver LAN8720ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver LAN8720プログラミング例が含まれています。

/ docディレクトリには、LibDriver LAN8720オフラインドキュメントが含まれています。

/ datasheetディレクトリには、LAN8720データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSMI, RMIIバステンプレートを参照して、指定したプラットフォームのSMI, RMIIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/lan8720/index.html](https://www.libdriver.com/docs/lan8720/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。