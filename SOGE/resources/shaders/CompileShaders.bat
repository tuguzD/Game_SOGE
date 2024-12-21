call dxc -T vs_6_0 -E VSMain simple.hlsl -Fo ../../../GAME/simple.vs.bin
call dxc -T ps_6_0 -E PSMain simple.hlsl -Fo ../../../GAME/simple.ps.bin
PAUSE
