API Protocol
========

| Endpoint URL      | Request type | Request data type | Response data type  | Description                                                                |
|-------------------|--------------|-------------------|---------------------|----------------------------------------------------------------------------|
| /                 | GET          |                   | string              | Check if server is up                                                      |
| /chgen            | POST         | JSON              | JSON                | Scan builds for changes, returns changelog and exports it into a text file |
| /transfer         | POST         | JSON              | JSON                | Scan builds for transfer changelog, returns changelog                      |
| /transfer         | GET          |                   | JSON                | Returns last scanned transfer changelog                                    |
| /submit           | POST         | JSON              | JSON                | Scan builds for submit changelog, returns changelog                        |
| /submit           | GET          |                   | JSON                | Returns last scanned submit changelog                                      |
