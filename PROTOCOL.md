API Protocol
========

| Endpoint URL        | Request type | Request data type | Response data type  | Description                                                                |
|---------------------|--------------|-------------------|---------------------|----------------------------------------------------------------------------|
| /                   | GET          |                   | string              | Check if server is up                                                      |
| /chgen              | POST         | JSON              | JSON                | Scan builds for changes, returns changelog and exports it into a text file |
| /transfer           | POST         | JSON              | JSON                | Scan builds for transfer changelog, returns changelog                      |
| /transfer           | GET          |                   | string              | Executes transfer based on last scanned transfer changelog                 |
| /transfer/changelog | GET          |                   | JSON                | Returns last scanned transfer changelog                                    |
| /submit             | POST         | JSON              | JSON                | Scan builds for submit changelog, returns changelog                        |
| /submit             | GET          |                   | string              | Executes submit based on last scanned submission changelog                 |
| /submit/changelog   | GET          |                   | JSON                | Returns last scanned submit changelog                                      |
