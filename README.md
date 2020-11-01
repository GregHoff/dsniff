# dsniff
Code from Dug Song updated to work with pcap 1.9 version on Kali 2019.4
------------------------------------------------------------------------
### Source code:
<https://salsa.debian.org/pkg-security-team/dsniff/-/archive/debian/master/dsniff-debian-master.tar.gz>

### Package versions included in Kali distribution:
> openssl 1.1.1g-1
>
> libssl-dev 1.1.1g-1
>
> libpcap-dev 1.9.1-4 

### Additional packages and version installed:
> libdb-dev 5.3.1+nmu1
>
> libnet1-dev 1.1.6+dfsg-3.1 
>
> libnids-dev 1.24-5
>
> quilt 0.66-2.1

### Check for latest versions and install missing packages:
> apt install -y make gcc g++ gdb libpcap-dev libnet1-dev libdb-dev libnids-dev libssl-dev openssl

### Apply debian patches if needed:
> apt install -y quilt
> 
> export QUILT_PATCHES=debian/patches
>
> quilt push -a

### Make but do not include webspy that tries to use a local Netscape browser:
> ./configure --without-x
>
> make 
