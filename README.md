
# Overview / 概要

The m5core2_gravsim project is a gravity simulator of planetary system for the [M5Stack Core 2](https://docs.m5stack.com/#/en/core/core2) .

\[日本語\]

m5core2_gravsim プロジェクトは、[M5Stack Core 2](https://docs.m5stack.com/#/en/core/core2) 用の惑星系の万有引力シミュレーターです。

# Video / 動画

![video](video.gif)

# Motivation / 開発のきっかけ

Have you ever thought how cool it'd be to hold the universe in the palm of your hand?
That little idea was where this whole project started.
I wanted to do more than just look at space—I wanted to bring the swirling dance of the planets to life in an interactive way, right on a tiny device like the M5Stack Core 2.
It's a fun way to play with their motion and grasp how gravity shapes our cosmos.

\[日本語\]

手のひらに宇宙を乗せられたら、ちょっとワクワクしませんか？
そんなちょっとした思いつきから、このプロジェクトは始まりました。
M5Stack Core 2みたいな小さなデバイスの上で、ただ眺めるだけじゃなく、惑星がくるくる回る宇宙のダンスをインタラクティブに再現してみたい。
天体の動きを楽しみながら、重力のルールを体験できる──そんな仕組みを作りたかったんです。

# Installation / インストール

1. Register an [M5Stack Community](https://community.m5stack.com/) account if you haven't already.

1. Launch the [M5Burner](https://docs.m5stack.com/en/download) .

1. Click `USER CUSTOM` in the lower left corner.

1. Log in to the M5Stack Community.

1. Click the `Share Burn` button.

1. Enter `UwE1Ekq5yMOozHfk` in the `Share Code` field.

1. Click the lightning icon :zap: .

\[日本語\]

1. [M5Stack Community](https://community.m5stack.com/) のアカウントをまだ登録していない場合は登録してください。

1. [M5Burner](https://docs.m5stack.com/en/download) を起動します。

1. 左下隅の `USER CUSTOM` をクリックします。

1. M5Stack Communityにログインします。

1. `Share Burn` ボタンをクリックします。

1. `Share Code` フィールドに `UwE1Ekq5yMOozHfk` と入力します。

1. 稲妻アイコン :zap: をクリックします。

# Installation for developers / インストール（開発者向け）

1. Clone the repository:
    ```sh
    git clone https://github.com/cubic9com/m5core2_gravsim.git
    cd m5core2_gravsim
    ```

1. Install [Visual Studio Code](https://code.visualstudio.com/) if you haven't already.

1. Open the project using Visual Studio Code:
    ```sh
    code .
    ```

1. Install `PlatformIO IDE` extension:
    - Go to the Extensions view (`Ctrl+Shift+X`).
    - Search for `PlatformIO IDE` and install it.

1. Open PlatformIO Home:
    - In the left Activity Bar, click PlatformIO icon.
    - In the left Primary Sidebar, fo to `QUICK ACCESS > PIO Home > Open`.

1. Open the project:
    - In the right Editor Group, click `Open Project` in the `PIO HOME` tab.
    - Select the cloned `m5core2_gravsim` project folder.

1. Build and upload the project:
    - In the left Primary Sidebar, go to `PROJECT TASKS > m5stack-core2 > General > Upload`.

\[日本語\]

1. リポジトリをクローンします:
    ```sh
    git clone https://github.com/cubic9com/m5core2_gravsim.git
    cd m5core2_gravsim
    ```

1. まだインストールしていない場合は、[Visual Studio Code](https://code.visualstudio.com/) をインストールします。

1. Visual Studio Code でプロジェクトを開きます:
    ```sh
    code .
    ```

1. `PlatformIO IDE` 拡張機能をインストールします:
    - 左サイドバーの「拡張機能」ビュー (`Ctrl+Shift+X`) を開きます。
    - `PlatformIO IDE` を検索してインストールします。

1. PlatformIO Home を開きます:
    - 左のアクティビティバーで PlatformIO アイコンをクリックします。
    - 左のメインサイドバーから `QUICK ACCESS > PIO Home > Open` を選択します。

1. プロジェクトを開きます:
    - 右のエディターグループ内の `PIO HOME` タブで `Open Project` をクリックします。
    - クローンした `m5core2_gravsim` プロジェクトフォルダーを選択します。

1. プロジェクトをビルド＆アップロードします:
    - 左のメインサイドバーから `PROJECT TASKS > m5stack-core2 > General > Upload` を選択します。

# License / ライセンス

Copyright (C) 2025, cubic9com All rights reserved.

This project is licensed under the MIT license.

See the `LICENSE` file for details.
