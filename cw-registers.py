BUFFER_NUM_SIZE = 1
REGSIZE         = 4
NUM_REGS        = 14

class Registers:
    DATA_SIZE = BUFFER_NUM_SIZE + REGSIZE*NUM_REGS

    def __init__(self, data):
        # We should have a buffer that is
        # 1 buffer number
        # sizeof(uint32_t) * registers
        assert(len(data) == Registers.DATA_SIZE)

        self.buffer_num = data[0]
        self.registers = []

        for i in range(0, NUM_REGS):
            # Parse the register in big-endian
            self.registers.append(data[1 + i*REGSIZE])
            for j in range(1, REGSIZE):
                self.registers[i] <<= 8;
                self.registers[i] += data[1 + i*REGSIZE + j];

    def get(self, i):
        """
        Get the value of a specific register i
        """

        if i < 0 or i >= NUM_REGS:
            raise Exception("OutOfRangeError: The '{}'-th does not exist".format(i))

        return self.registers[i]

    def get_buffer_num(self):
        """
        Get the register buffer specifier of this list of registers
        """

        return self.buffer_num

    def print(self):
        """
        Print the list of registers
        """

        print("Registers:")
        for i, value in enumerate(self.registers):
            print("  {:02}: {:32x}".format(i, value))