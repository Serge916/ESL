tile-add    0 40000000 10000000 128k 32 16
tile-add    1 40000000 10000000 128k 32 16
tile-add    2 40000000 10000000 128k 32 16
tile-tdm-os 0 2000
tile-tdm-os 1 2000
tile-tdm-os 2 2000
tile-mem-os 0 32k
tile-mem-os 1 32k
tile-mem-os 2 32k
tile-mem-add  0 1 32k 0x8000
tile-mem-data 0 1 vep_1/partition_0_1/out.hex
# global & partition timers
tile-mmu-add  0 1 0 0x08FC0000 0x08FC0000 0xFFFEFFF8
# ARM-RISCV stdin/out for system (vp_ios)
tile-mmu-add  0 1 1 0xB0000000 0xB0001800 0xFFFFFE00
# ARM-RISCV stdout for the partition (vp_user)
tile-mmu-add  0 1 2 0xB0000800 0xB0003000 0xFFFFF800
# memory regions shared by the VEP in shared memories
tile-mmu-add  0 1 3 0xB0009000 0xB0009000 0xFFFFF000
tile-mmu-add  0 1 4 0xB000A000 0xB000A000 0xFFFFE000
tile-mmu-add  0 1 5 0xB000C000 0xB000C000 0xFFFFC000
tile-mmu-add  0 1 6 0xB0010000 0xB0010000 0xFFFF0000
# memory regions shared by partitions in instruction/data memories
tile-mmu-add  0 1 7 0x90010000 0x90010000 0xFFFF0000
tile-tdm-add  0 0 5000
tile-tdm-add  0 1 58000
tile-mem-add  1 1 32k 0x8000
tile-mem-data 1 1 vep_1/partition_1_1/out.hex
# global & partition timers
tile-mmu-add  1 1 0 0x08FC0000 0x08FC0000 0xFFFEFFF8
# ARM-RISCV stdin/out for system (vp_ios)
tile-mmu-add  1 1 1 0xB0000000 0xB0002000 0xFFFFFE00
# ARM-RISCV stdout for the partition (vp_user)
tile-mmu-add  1 1 2 0xB0000800 0xB0005000 0xFFFFF800
# memory regions shared by the VEP in shared memories
tile-mmu-add  1 1 3 0xB0009000 0xB0009000 0xFFFFF000
tile-mmu-add  1 1 4 0xB000A000 0xB000A000 0xFFFFE000
tile-mmu-add  1 1 5 0xB000C000 0xB000C000 0xFFFFC000
tile-mmu-add  1 1 6 0xB0010000 0xB0010000 0xFFFF0000
# memory regions shared by partitions in instruction/data memories
tile-mmu-add  1 1 7 0x90010000 0x90010000 0xFFFF0000
tile-mem-add  1 2 64k 0x10000
tile-mem-data 1 2 vep_1/partition_1_2/out.hex
# global & partition timers
tile-mmu-add  1 2 0 0x08FC0000 0x08FC0000 0xFFFEFFF8
# ARM-RISCV stdin/out for system (vp_ios)
tile-mmu-add  1 2 1 0xB0000000 0xB0002200 0xFFFFFE00
# ARM-RISCV stdout for the partition (vp_user)
tile-mmu-add  1 2 2 0xB0000800 0xB0005800 0xFFFFF800
# memory regions shared by the VEP in shared memories
tile-mmu-add  1 2 3 0xB0009000 0xB0009000 0xFFFFF000
tile-mmu-add  1 2 4 0xB000A000 0xB000A000 0xFFFFE000
tile-mmu-add  1 2 5 0xB000C000 0xB000C000 0xFFFFC000
tile-mmu-add  1 2 6 0xB0010000 0xB0010000 0xFFFF0000
# memory regions shared by partitions in instruction/data memories
tile-mmu-add  1 2 7 0x90010000 0x90010000 0xFFFF0000
tile-tdm-add  1 0 5000
tile-tdm-add  1 1 58000
tile-mem-add  2 1 32k 0x8000
tile-mem-data 2 1 vep_1/partition_2_1/out.hex
# global & partition timers
tile-mmu-add  2 1 0 0x08FC0000 0x08FC0000 0xFFFEFFF8
# ARM-RISCV stdin/out for system (vp_ios)
tile-mmu-add  2 1 1 0xB0000000 0xB0002800 0xFFFFFE00
# ARM-RISCV stdout for the partition (vp_user)
tile-mmu-add  2 1 2 0xB0000800 0xB0007000 0xFFFFF800
# memory regions shared by the VEP in shared memories
tile-mmu-add  2 1 3 0xB0009000 0xB0009000 0xFFFFF000
tile-mmu-add  2 1 4 0xB000A000 0xB000A000 0xFFFFE000
tile-mmu-add  2 1 5 0xB000C000 0xB000C000 0xFFFFC000
tile-mmu-add  2 1 6 0xB0010000 0xB0010000 0xFFFF0000
# memory regions shared by partitions in instruction/data memories
tile-mmu-add  2 1 7 0x90010000 0x90010000 0xFFFF0000
tile-tdm-add  2 0 5000
tile-tdm-add  2 1 58000
