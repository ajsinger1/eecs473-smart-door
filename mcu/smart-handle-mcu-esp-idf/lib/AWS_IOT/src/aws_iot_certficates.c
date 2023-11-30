/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * Additions Copyright 2016 Espressif Systems (Shanghai) PTE LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file aws_iot_certifcates.c
 * @brief File to store the AWS certificates in the form of arrays
 */

#ifdef __cplusplus
extern "C" {
#endif

const char aws_root_ca_pem[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n\
rqXRfboQnoZsG4q5WTP468SQvvG5\n\
-----END CERTIFICATE-----\n"};

const char certificate_pem_crt[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDWjCCAkKgAwIBAgIVAJ1Oj8EIrmfc6osV+0Dxs09dyZePMA0GCSqGSIb3DQEB\n\
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n\
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMzExMTExODIw\n\
MDFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n\
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDgLjm7g9xC2O4US1tZ\n\
q6KE1NCZafuLw4Wc50e1E4tAomlaUpaPnp0O1nx9ozZMrUQcutCpQPnd13/My+iy\n\
jbitUlW9QqF+aA3SZ0QDs7wtHrj2QELGqGvTmiamBfP3iMAIRPvU9MZwR2ig1F7V\n\
a45qCx9/bv/lLws6Rn7/H8huNgYEXWWtXg+o6Ob7EvU0CQeJtKKhEFlNj9dqR+Ft\n\
vkX+TcefCvBfRWoNMnvxgHxCtGWwKJYqkFi2EXOdeEqP6DDrN6t9zxmSUsEkMiVO\n\
sZWyc/rMYI4u2lSsIRDCOUXDZAF9giZLn51agsR/BZuemMbLma4ElW+x+ocr/n3Q\n\
Ikl1AgMBAAGjYDBeMB8GA1UdIwQYMBaAFImLYH/bQvvrIAGQUk8YErF7zAB3MB0G\n\
A1UdDgQWBBRO8CWCiyHXWq5XRkreWnex6NJ+LDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n\
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAoWzFe7Y4C9v3dWSHblyVLqVd\n\
oWB65nCfUohjJ/DYOeBQXIUkVCr5ZMMxF69AutVrr4rP/eWQQT/14LwnXtDju8qN\n\
WAgkz/ES5LO64vNbHHKz0DPT8nXqmy5KQvV8iAXFDhyA1SdcVpKipyZCco7+UCFU\n\
4Lx4LByvht+02LHDfrBgSwbLmU0Rl+XpCjAf+UcnlfCRp5Koyt8824+kqvF1Wkji\n\
wN5SZ7sokHHPyNC9Cw2Qv3Bxi+qN4Ao7FN19V2zivjZG4cLVXIhv/P1+Vkn1igE9\n\
O6tK5FQMOfJnQ0pNw8wnFq+ovCqxRgr7wvSPIbahrS8tIeIThpR847rdMt1Anw==\n\
-----END CERTIFICATE-----\n"};

const char private_pem_key[] = {"-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpAIBAAKCAQEA4C45u4PcQtjuFEtbWauihNTQmWn7i8OFnOdHtROLQKJpWlKW\n\
j56dDtZ8faM2TK1EHLrQqUD53dd/zMvoso24rVJVvUKhfmgN0mdEA7O8LR649kBC\n\
xqhr05ompgXz94jACET71PTGcEdooNRe1WuOagsff27/5S8LOkZ+/x/IbjYGBF1l\n\
rV4PqOjm+xL1NAkHibSioRBZTY/Xakfhbb5F/k3HnwrwX0VqDTJ78YB8QrRlsCiW\n\
KpBYthFznXhKj+gw6zerfc8ZklLBJDIlTrGVsnP6zGCOLtpUrCEQwjlFw2QBfYIm\n\
S5+dWoLEfwWbnpjGy5muBJVvsfqHK/590CJJdQIDAQABAoIBAEpDReC2R6rlTcjv\n\
Ra2oy8eQ8CFCGJAuDv7xJrH3bXPV2WyQW0vsUT7vEoJUJNK1uxCBWkoXOi6hCrb+\n\
/XDYvcq8saZmgjTrp1iB2OgZDEcuHDuHAhfpBv8r2423/x+K74JNx9a911aucRyF\n\
/XxZiu3aDW8biC2VrfR6EevVzYGkSOjy4HjIR8K0Xp+2+kJ9GoDKkCnkdUeFJiZC\n\
Qxbhypjxo7swqPIx0O0K0yEmzv7osWBMmOAIjKlwYK07UJ96RR/zFsuD3RT8e2fa\n\
GuIrMQBjn5NWvjYyiRn+iI3f+di9H08d7o0QhNb9fCrFAlU1EMlm0kNyniwnT8vt\n\
fSW8MIECgYEA+xp61pD0X5lmA1zxtN9UpoarcLlA+gNpcZbwlbjBDbX9tpnUSQQ9\n\
OdldimrsX9JYmXquqTWrPoAlAfO3s0+9jzVxNkXzBmyG6uSJn0xgkwczd30F9Fu2\n\
HbsIdmcNDiD2Ce6YM5/6k8Lm+M+j0H3r/f6+42px/6Wz1uanW3UltHECgYEA5I1Y\n\
b8vFihpbs3uJld/5KR614uss2AOtWNaFGk83K8A7inFK4wkd/Wq2uxhTcXACLTha\n\
kbRIW4mP+KkFdOdRCtHQSJE71zw6uO4ovSTvsNCyBW7VkjI4w6KjRJ/7wZ0coGn9\n\
qvDlqFzhrw01EOKoO5mEjNVeuzkvdZSK4zUql0UCgYEA3Qa+75Vida/aGsW6QGKA\n\
6aAK0ktOxYMxUnnxRWj+DaqPLCcVtDvWBNCvRMK/U80cjeRuT6VdGcPET4aOwXhE\n\
PAVnpMVHmPT4gVQvS5f4PhMNm3TiQ+hiUqMqXMZyuGgNCksJkA+Wim/5zTNAV6ov\n\
i0CsGBO/Sq85sMUQtTFVwhECgYAmcPux+hUksGBCJQI0OjK/TtIK88FcJ+otKg/D\n\
KBXAfxnEpF+TJkJsEFnhyCqoxs0m6/aX0TnmY5Haohsv2YRRDI861Wq6PgAmgLBL\n\
d/30wHVg4o57g4T2qtFJRptiqllZRdqLuO73zX+MIO2PBICFXcHkqjK/zNOFtFFT\n\
++pI/QKBgQDbflc8Bm/NVPq3i7q+HTwkcPLazkoutkI9pXAEIUt9z2ayZGdwPIe/\n\
I+WKvci2A461aAP+Y5gMZo9VZ3Bz/+QvmAwRRwlNKuZj4UWkjeCfWHdsy0rHUqsx\n\
+omnyYS955SM3t+Set2qZx9Eh9V6shOEUt8PqDEMHD7zFAsHHXIHAA==\n\
-----END RSA PRIVATE KEY-----\n"};

#ifdef __cplusplus
}
#endif
