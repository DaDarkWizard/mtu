# MTU command tool

This tool makes doing things with MTU a little easier.
Currently only handles the VPN connection.

A config file is required with your MTU username and password like so:

mtu_id username
mtu_password fullPassword

This file is stored in C:\Users\Username\.mtu\config

Usage:

Currently only the following command are supported:

mtu vpn connect|start

mtu vpn stop|disconnect|close

The f5fpc (BIG IP client) must be on your path.
