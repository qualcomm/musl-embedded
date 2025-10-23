# Notes on MUSL for Embedded

This MUSL fork can be built for either Linux or a baremetal environment,
meaning it can run without an underlying operating system.

When targeting baremetal, MUSL embedded provides a simplified implementation
that supports essential C APIs, deliberately omitting POSIX functions and
other Linux-specific features that depend on system calls.

As a result, it is not thread-safe, and certain functions are either implemented
as no-ops, such as raise(), or result in infinite loops, such as exit().
Additionally, errno is not supported, and the errno.h header should not be
included when building code for baremetal targets. This means that standard
libc APIs that typically set errno are either unsupported or do not modify errno.
If needed, users may define and manage their own errno variable within
their applications.
