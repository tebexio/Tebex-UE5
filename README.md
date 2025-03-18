# Tebex for Unreal Engine 5

This is the official example [Tebex](https://tebex.io/) plugin for Unreal Engine 5. Tebex is a gaming-focused payments processor that 
allows you to easily set up your game for monetization.

The example plugin adds the following which will streamline your Tebex integration:
- Fully-featured [Headless API](https://docs.tebex.io/developers/headless-api/overview) for browsing packages and creating baskets on Tebex stores
- Core implementation of our [Plugin API](https://docs.tebex.io/plugin) for package delivery after payment
- An example game **Plugin** that implements required core Tebex logic.

## What is a Tebex Project?
Tebex allows Creators (game studios and/or players) to set up a digital storefront and sell **Packages**.

**Packages** can include deliverables such as **Game Commands**, **License Keys**, **Downloads**, and more which are given to the purchaser.

The storefront can both be a website and your own in-game GUI using the **Headless API**.

Depending on your requirements, you can implement various elements of this example plugin into your game to support Tebex quickly and easily.

## Installation

To begin, clone this repository to your Unreal Engine game's *Plugins* directory.

## Integration Guide

A minimal Tebex integration is expected to:
1. Connect to a store via a secret key [accessible from your Tebex account](https://tebex.io/)
2. Allow the player to browse and select packages in that store
3. Add those packages to a Basket
4. Direct the user to check out (performed in the browser)
5. Deliver purchases to the player after payment in some way

Most typically, purchases are delivered via **Game Commands** (such as adding money, adding items, etc.) that are queued and executed every few minutes. 

This repository includes an example TebexPlugin that will execute game commands.

For more advanced delivery methods, Tebex also provides [Webhooks](https://docs.tebex.io/developers/webhooks/overview) to allow fully custom handling of purchases on your own backend. 

### 1. TebexHeadlessAPI

The Headless API allows you to browse a store and build a basket of items that can be purchased by the player. These are the same APIs used by our webstores.

### 2. TebexPluginAPI

The Plugin API allows you to check for deliverables that are due / purchases that have been made and then mark them completed once delivered successfully.

### 3. TebexPlugin

Our implementation in **TebexPlugin** details how your game should interact with our APIs to deliver purchases in-game via console commands.

This assumes we are in a _server environment_ that the game studio or the store creator controls.

If your setup does not deliver commands in-game, you can use our [Webhooks](https://docs.tebex.io/developers/webhooks/overview) to receive real-time purchase notifications and events on your own backend to deliver purchases.

## Contributions
We welcome contributions from the community. Please refer to the `CONTRIBUTING.md` file for more details. By submitting code to us, you agree to the terms set out in the CONTRIBUTING.md file

## Support
This repository is only used for bug reports via GitHub Issues. If you have found a bug, please [open an issue](https://github.com/tebexio/Tebex-UE5/issues).

If you are a user requiring support for Tebex, please contact us at https://tebex.io/contact