# dsniff
Code from Dug Song updated to work with pcap 1.9 version on Kali 2019.4
------------------------------------------------------------------------
### Source code:
<https://salsa.debian.org/pkg-security-team/dsniff/-/archive/debian/master/dsniff-debian-master.tar.gz>

### Packages included in Kali distribution:
> openssl 1.1.1g-1
>
> libpcap-dev 1.9.1-4 

### Additional packages installed:
> libdb-dev 5.3.1+nmu1
>
> libnet1-dev 1.1.6+dfsg-3.1 
>
> libnids-dev 1.24-5
>
> quilt 0.66-2.1

### Apply debian patches:
> export QUILT_PATCHES=debian/patches
>
> quilt push -a

### Make:
> ./configure
>
> make libmissing.a
>
> make dnsspoof
