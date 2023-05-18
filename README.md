# rsync_client

[librsync](https://github.com/librsync/librsync) demonstration

```shell
Usage: rsync_client [OPTIONS]... SOURCE OUTPUT [SECOND_SOURCE]
Allowed options:
  --help                produce help message
  --source arg
  --second_source arg   must be set if operation is signature or operation is 
                        patch
  --output arg
  --signature           generate the signature of a source file and write it 
                        out to output
  --delta               generate a delta between a signature and a source file 
                        into a output file
  --patch               apply a patch, relative to a source file, into a output
                        file
```
