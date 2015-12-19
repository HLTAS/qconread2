# qconread2

qconread2 is a GUI frontend for reading log files generated by [taslogger](https://github.com/HLTAS/taslogger). This application uses the Qt toolkit to build its user interface.

## Why?

The purpose of qconread2 is to reduce some friction one may encounter in the TASing process. One of the most frustrating aspects of TASing is to "troubleshoot" the script, so to speak. That is to say, to figure out why a particular iteration of the script does not work as expected, or to understand why some unusual behaviour occurs, or to ascertain what really happens when a desync occurs. In these cases, qconread2 can prove useful as a tool to illuminate the problems at hand.

Needless to say, qconread2 is vital in TASing intense frame-perfect sequences. It can be hard or impossible to observe what exactly happens by merely running the script. This is particularly true when doing complex object manoeuvring where the velocity can change wildly over the span of a few frames. When an issue arises in such a tight sequence, you do not want to figure out what went wrong by guesswork!

In general, qconread2 is a tool to help wrapping your mind around what happened in your TASes. Many details can be missed without the logs available for offline viewing.

## Limitations

qconread2 does not give you all the answers. When faced with an issue, an intuition for Half-Life physics is always vital in understanding the situation, and experience can guide your hand in fixing the scripts.

qconread2 stores the entire log in memory. This is rarely an issue. The in-memory representation of the log is more efficient than the content of the log file itself. However, when there is a enormous number of frames in the log, qconread2 will become memory intensive.

## Building

1. Install [Qt5](http://www.qt.io) and [RapidJSON](https://github.com/miloyip/rapidjson/).
2. Run `git submodule update --init`.
3. Create a `build` directory alongside the `src` directory.
4. Change into the `build` directory.
5. Run `cmake -DRapidJSON_ROOT=/path/to/rapidjson/base/dir ..`
6. Run `make -j` or build the generated Visual Studio solution.
