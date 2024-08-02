#include <pgmspace.h>

#define SECRET
#define THINGNAME "test"

const char WIFI_SSID[] = "Wokwi-GUEST";
const char WIFI_PASSWORD[] = "";
const char AWS_IOT_ENDPOINT[] = "a1ecz2lixjwnxr-ats.iot.us-east-1.amazonaws.com";

//Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

//Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIULMFtgMP1JNAGUv6lD1RD5kc7WzwwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MDMwOTA5NDgw
OFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMzA7SJsrF480cGMf7Fu
5O81YNKtNvTrYRfmylEeSL4OZOQCAW9CPzOuPXPCXw0UFqfdnu8E7YZf/wRsge7S
kbIaOV3NHJBYYO5KXtfEum9tywKRn6bu/3UFHyNTfaSJ6vwT4bw94fvL3GqvLJYP
GUwgWxW8eBV6VA+s2JGDIBxUkutAOLTgqy6pUC8gStdtt4+aGMFxKyRq3HgOEkaL
WXbCwULIlEW9QAVuRwnTjMXjQPisdoU0657mrjTfM8L6pxE4ZUcGEGxa5RZ+25Tz
4iblydIPFnbfq8Ju/eCyj2EHJDTU6nMmgbmqHWAg8GMXXsFDydYRM3jzO2P6vPU0
jFcCAwEAAaNgMF4wHwYDVR0jBBgwFoAUFTdvEq8OTcvxASWcHZUT1lbroS0wHQYD
VR0OBBYEFCTVZKwhixvOrwb2nPCkMbOsJlKIMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQB6enZszI1qM83rRZlef44edUeS
LqBP8MBpg+uVF6dDJUJvzP7WTE7f0nLdR9c6RTm48ValJYmxKSzf2wOeJRLPSyBJ
9sLRCNyCkFcxrdwd+5vwcHmBs68ZmZpD4yq2p9RUY/WntSPPRhtZYzmnuZMS7/98
7/Udloe00r1hptRAe1koa6Gc6eWH9S0epx9cUokF1Vd4KiRltLvrN4FuhwiRGWVj
s2JySiXkFh5G+QrF7lHNqI7AuHmMTh1hgvGqpGieQ/2kc5bQMapYRQh1bttFPhr0
f5HJnsw+JYKNglg5B0VA8QRUwqBUkEXa9piDllnWEKcTYpvAbbOFeD9jD008
-----END CERTIFICATE-----
)KEY";

//Device private key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAzMDtImysXjzRwYx/sW7k7zVg0q029OthF+bKUR5Ivg5k5AIB
b0I/M649c8JfDRQWp92e7wTthl//BGyB7tKRsho5Xc0ckFhg7kpe18S6b23LApGf
pu7/dQUfI1N9pInq/BPhvD3h+8vcaq8slg8ZTCBbFbx4FXpUD6zYkYMgHFSS60A4
tOCrLqlQLyBK1223j5oYwXErJGrceA4SRotZdsLBQsiURb1ABW5HCdOMxeNA+Kx2
hTTrnuauNN8zwvqnEThlRwYQbFrlFn7blPPiJuXJ0g8Wdt+rwm794LKPYQckNNTq
cyaBuaodYCDwYxdewUPJ1hEzePM7Y/q89TSMVwIDAQABAoIBAQCiYSYutMyF9+nJ
eykRiAabBlQBAEmjNsBLekZcyo2Z3ER65nRIzdrdm4M0uKM09gd5nTKcD+ZnON4/
neX1hpzxCAcSssXCW7SThfttVqEcGZB0OxTICKYjIaquE1qDELWAsCvHY7RCQkxN
dxr+7n57dvWFaXA6aEceH0+pJrpie+zV0YItB5/qnJU10FsisSprxYBZINzgVPJj
65DBwdnPXWDIaD7OsynoPsk/mLK9bVmgRAh2h9z0YxpXLw9rOjdQYzANeXSnCo8S
vBThwdKDYgd6fYIjRH1GB47zoyyOjFOQ7u2hGSTg6OkCU7oBv/eu9TGZfJ7nBYve
WE+X2BzBAoGBAOe+xhWEBE0OksYfSZBjLQAZvCoePhnFE87smj+9JnZ6wStUPOsm
LIhekanF2sDR8cNeihlk2sxDS5TejaLtPELlDkvZt06UKjJHaMgHPI0zbfObztLI
mU0bU7BeivgCXnhaYd5mJvNWVF0jEwrEuFH19Pa7/H1dlJgQxTPWgiSHAoGBAOIu
9TeFVbVnnUDx+PJBXFLgU/KQolNwDuZpd2elXj/JEfyrduiFK2Ld0Um/BZ8C/EhA
Iu557SH3lmqHQwC95WOZInZq/Dbh+eqCqmz4/a1UOQWkbjTrhpsgQEnqrw/30bDN
XV82pRL7uOwR2AUOVrqF87WQozcUObY1yHb7uh2xAoGBAJGbGug/11f3FR8oX1c6
CPcsND6eW9RjMAvuOwhbNwRwIliwB8qTZJ2WFZ1+8+/8uRNQniJQbOy0R5PUtVcu
CZPaST49H+BeVZjbpyku6supCadrOOrVIQUCa3XAIT6sdmDWIPHUaxqvHjiapgc5
DLpqeCxidsTp/EgDGXR1jbWXAoGABn0CW81fYAzQ3gQS+CAX3ejBf7vWbD5Srxln
a3m68ml6Mp1Yk+L+E5/5WPw51Q3o3sm4Ns8Ylljb38JxkZvg8LJ74pm/EnP5D1kg
OnAKFubLm/XTk4EOI3URYw1waCzbvw6lFw4iPiEqdNsuLzbyFfexvgugIvxZi3iP
f5pJBIECgYEAhQHNu5uvUiyENOC7j74GpPHvAuPfKX/vbTBAATNt0h/Xrmv+TQB/
n1ebZWigpLlzevBxgzgtFnmfmUjZbQNlpAWALNgB0rqAKx1QQEaqL3m+yFSVh2j9
dTgD7dU6zAXKkO/IA8CErIqdEq4O8pkmmzX98Hs5VUJBSSSRT+YXz3g=
-----END RSA PRIVATE KEY-----
)KEY";