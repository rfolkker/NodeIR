# NodeIR

Repository for the NodeIR code in my attempt at a wireless remote control.

## Sample Post

The default command is to post irCmd.  Use this with a body of irData (Hex String), and irManufacter (string).
*irCmd*

```javascript
{
    "irData": "E0E0807F",
    "irManufacturer": "Samsung"
}
```

To initialize your wireless network.  must be done if power is reset.
*irInit*

```javascript
{
    "irSSID": "SSID Name",
    "irPWD": "Wireless password",
    "irAuthKey": "app key to OAuth Key",
    "irAuthPath": "URI path"
}
```
