[Hello Libtorrent : Dump Torrent file]

# What is torrent file (metainfo files)

A Torrent file is used by bittorrent client.  
This file data contain a information about to connect anotoher peer.
and target file's information.

- announce
  tracker server url

- info.piece length
   piece size 

- info.piece length  
   sha1 hash per piece.

- info.length
   file size

- info.name
   suggestion file or directory name

- info.path  
   suggestion file path

And This Specification is defined by http://bittorrent.org/beps/bep_0003.html and http://bittorrent.org/beps/bep_0052.html and etc..


Torrent file is described at bencode format.


# About Piece
In Bittorrent client, data is divided and managed. 
Torrent file 's "piece size" parameter is this divided data 's size. 

and  "piece" parameter is joined manufactured data which is 's SHA1 Hash per this divided data.


# Check Torrentfile 

I explained about the bencode before. 
https://dev.to/kyorohiro/hello-libtorrent-bencode-4ih3

I write a code for dumping a torrent file.
And Dump there.

### Version 1 Simple Pattern 

```
  {
    <announce> : 'http://files.publicdomaintorrents.com/bt/announce.php'
    <creation date> : '1136381877'
    <info> : 
      {
        <length> : '251828879'
        <name> : 'Warning_from_Space.mp4'
        <piece length> : '262144'
        <pieces> : '(size:19220)F6FA14D15AE940EDFD4C99BD41267B9B4688CDDC1D34DDFEE1ED86F085593FA20A0D0FACBE06AD3EF0F9AD7ABEE9A63B6FD6'
      }
  }
```

### Version 1 Files

```
  {
    <announce> : 'http://tracker.frostwire.com:6969/announce'
    <creation date> : '1231188572'
    <info> : 
      {
        <files> : [  
            {
              <length> : '928670754'
              <path> : [  'Big_Buck_Bunny_1080p_surround_FrostWire.com.avi'  ]
            }
            {
              <length> : '5008'
              <path> : [  'PROMOTE_YOUR_CONTENT_ON_FROSTWIRE_01_06_09.txt'  ]
            }
            {
              <length> : '3456234'
              <path> : [  'Pressrelease_BickBuckBunny_premiere.pdf'  ]
            }
            {
              <length> : '180'
              <path> : [  'license.txt'  ]
            }  ]
        <name> : 'Big_Buck_Bunny_1080p_surround_frostclick.com_frostwire.com'
        <piece length> : '262144'
        <pieces> : '(size:71120)A14071E4D8FFA946B6DC900B8E9490004779FFD6D1C17059A6BC2BE4348D7C0CBB5C5DE0DBE8E92692297109A4406DF8AB30'
      }
  }
```


### Verions 2

```

  {
    <created by> : 'libtorrent'
    <creation date> : '1554199319'
    <info> : 
      {
        <file tree> : 
          {
            <stress_test0> : 
              {
                <> : 
                  {
                    <length> : '1048576000'
                    <pieces root> : '(size:32)6DAE0824F0DC38043079DFACCE900CB7B24F308868C2DFF3B23F9AF2F218903D'
                  }
              }
            <stress_test1> : 
              {
                <> : 
                  {
                    <length> : '1048576200'
                    <pieces root> : '(size:32)5B9566A4D5301882644C1C6A083CB05F07F6370F887D35424A3D50E6D0559A3B'
                  }
              }
            <stress_test2> : 
              {
                <> : 
                  {
                    <length> : '1048576400'
                    <pieces root> : '(size:32)A26B08DCA3D9A762CED3BB56DD7A78E7FB9ACEFAEA5A2A41DE44C5C9169325F2'
                  }
              }
          }
        <meta version> : '2'
        <name> : 'test'
        <piece length> : '1048576'
      }
    <piece layers> : 
      {
        <(size:32)5B9566A4D5301882644C1C6A083CB05F07F6370F887D35424A3D50E6D0559A3B> : '(size:32032)34ACC071F1AA09FC913117D8C90147185CD0B93F1FA33BAF7880B8E0E1083EC2AE96A9D9B5FA1FF0165C5295E391C306AE87'
        <(size:32)6DAE0824F0DC38043079DFACCE900CB7B24F308868C2DFF3B23F9AF2F218903D> : '(size:32000)EDC7C54F3C543DCBD9A62B204F44DB2B1A95E15533A3B400952D092B8B5015E4922209676C9F49C5393C0E7A524933F431AA'
        <(size:32)A26B08DCA3D9A762CED3BB56DD7A78E7FB9ACEFAEA5A2A41DE44C5C9169325F2> : '(size:32032)AB93AAA8AB5C4480812C2BC8EC27044AC401F1F0FB2EDF74086F1CDF67D646C50CF06E862863599E4C76EB7B8AD6EA8BA572'
      }
  }
```



# Source Code

https://github.com/kyorohiro/hello_libtorrent
app/main_bdecode_dump.cpp 


# REF

http://bittorrent.org/beps/bep_0003.html
http://bittorrent.org/beps/bep_0052.html



# My Memo About Libtorrent And Index 
(0) code-server for libtorrent 2.0 
https://dev.to/kyorohiro/code-server-for-libtorrent-2-0-1m85


