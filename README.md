![Tebex Logo](https://www.tebex.io/_nuxt/logo.BCN2mLkL.svg)
# Tebex for Unreal Engine 5

This is the official [Tebex](https://tebex.io/) SDK for Unreal Engine 5. Tebex is a gaming-focused payments processor that 
allows you to easily set up your game for monetization.

## Installation

1. Open your game's solution in your IDE
2. Clone this repository to your game's `Plugins` folder

⚠️ Depending on your IDE, you may need to re-index your project before building. All source files may not immediately be recognized and added to your game's solution and cause compilation failures. Ensure all files in the `Plugins/TebexUE5` folder are indexed in your IDE.

- Rider: In **File System** view, right-click any source code files in `Plugins/TebexUE5` suffixed with `no index` and click **Include**.

## Features

- **Headless API Integration**: A C++ implementation for [Headless API](https://docs.tebex.io/developers/headless-api/overview) used for retrieving packages and creating baskets
- **Plugin API Support**: If your game delivers purchases through commands, our [Plugin API](https://docs.tebex.io/plugin) enables automated command execution after payment.
- **Example Plugin**: We've included an example TebexPlugin for Unreal Engine to demonstrate how a Tebex integration should operate.

Our SDK is built on **Unreal Engine 5.5.4**, however most SDK code should be modular between Unreal Engine versions with minimal required changes.

## What is Tebex?
[Tebex](https://tebex.io) allows game studios and/or players to set up a digital storefront and sell **Packages**. **Packages** can include deliverables such as **Game Commands**, **License Keys**, **Downloads**, and more.

Your storefront may be a website or fully integrated into your game using **Headless API**.

Originally known as Buycraft, Tebex supports an expanding library of games, including:
- Minecraft
- Rust
- ARK: Survival Evolved
- FiveM
- Garry's Mod
- Unturned

### Tebex Unreal Engine SDK
This repository serves as our full Tebex SDK for Unreal Engine, allowing for quick and seamless integration into your game. It is designed to be forked and modified for your particular engine version and your game's requirements.

## Integration Guide

A minimal Tebex integration is expected to:
1. Connect to a store via a secret key [accessible from your Tebex account](https://tebex.io/)
2. Allow the player to browse and select packages in that store
3. Add those packages to a Basket
4. Direct the user to check out (performed in the browser)
5. Deliver purchases to the player after payment in some way

Most typically, purchases are delivered via **Game Commands** (such as adding money, adding items, etc.) that are queued and executed every few minutes. 

This repository includes an example **TebexPlugin** that will execute game commands.

For more advanced delivery methods, Tebex also provides [Webhooks](https://docs.tebex.io/developers/webhooks/overview) to allow fully custom handling of purchases on your own backend. 

### 1. TebexHeadlessAPI

The Headless API allows you to browse a store and build a basket of items that can be purchased by the player. These are the same APIs used on our store websites.

### 2. TebexPluginAPI

The Plugin API allows you to check for deliverables that are due / purchases that have been made and then mark them completed once delivered successfully via a command.

### 3. TebexPlugin

Our implementation in **TebexPlugin** details how your game should interact with our APIs to deliver purchases in-game via console commands.

This assumes we are in a _server environment_ that the game studio or the store creator controls.

If your setup does not deliver commands in-game, you can use our [Webhooks](https://docs.tebex.io/developers/webhooks/overview) to receive real-time purchase notifications and events on your own backend to deliver purchases.

## Contributions
We welcome contributions from the community. Please refer to the `CONTRIBUTING.md` file for more details. By submitting code to us, you agree to the terms set out in the `CONTRIBUTING.md` file

## Support
This repository is only used for bug reports via GitHub Issues. If you have found a bug, please [open an issue](https://github.com/tebexio/Tebex-UE5/issues).

If you are a user requiring support for Tebex, please contact us at https://tebex.io/contact
