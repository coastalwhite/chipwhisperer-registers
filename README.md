# ChipWhisperer Registers

Fetch and record register values of ChipWhisperer targets. Aims to have the
minimal effect in changing the registers when measuring, thus lends itself to
registers difference analysis.

## Usage

Provided is a interaction between the target code in `cw-registers.h` and the
processing code in `cw-registers.py`.

### C

All functionality is found in `cw-registers.h`. In general, the process in *C*
exists out of two steps: *capturing* and *putting*. These are done by
`capture_registers(n)` and `put_captured_registers(n)`, respectively. `n` is an
identifier of the captured registers. If you just need one set of registers `0`
is fine.

```c
#include "cw-registers.h"

capture_registers(0);
put_captured_registers(0);
```

If you want to capture the effect of a set of instructions, you want to give
them different identifiers.

```c
#include "cw-registers.h"

capture_registers(0);
// ...set of instructions of interest
capture_registers(1);

put_captured_registers(0);
put_captured_registers(1);
```

The default number of buffers available for registers is 2. If you need more
than 2 buffers, you define `CW_REGISTERS_NUM_BUFFERS` before including
`cw-registers.h`.

```c
#define CW_REGISTERS_NUM_BUFFERS 4
#include "cw-registers.h"

capture_registers(0);
// ...set of instructions of interest (part 1)
capture_registers(1);
// ...set of instructions of interest (part 2)
capture_registers(2);
// ...set of instructions of interest (part 3)
capture_registers(3);

put_captured_registers(0);
put_captured_registers(1);
put_captured_registers(2);
put_captured_registers(3);
```

### Python

The python module provides one class `Registers`. To initialize it, you pass
the data returned from a `r` SimpleSerial command. It will then parse
everything and you will be able to requests individual registers, the buffer
number or the entire array of registers.

```python
import struct
from cw-registers import Registers
scope.arm()

# Send a SimpleSerial command to trigger the command
target.simpleserial_write('p', plain_text)

# Read the registers command
data = target.simpleserial_read('r', Registers.DATA_SIZE)

# Convert the output into registers
output = struct.unpack("<I", val['payload'])[0]
registers = Registers(output)
```
