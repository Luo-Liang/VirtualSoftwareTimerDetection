# VirtualSoftwareTimerDetection
Detects whether a program is doing VST maliciously with performance counters.

# The repo includes:
VSDtection: software virtual timer program

# Workloads
cryptograph (Malicious):
    * aes_test + probe thread
    * des_test + probe thread
    * hmac_test + probe thread
    * rsa_test + probe thread

parsec-3.0 (benign):
    * original parsec workloads
