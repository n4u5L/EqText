#include <ntddk.h>

// 定义全局变量存储0x66端口上一次的值
ULONG previousPort66Value = 0;

// 定义函数读取端口值
ULONG ReadPort(ULONG portAddress) {
    return READ_PORT_UCHAR(portAddress)
}

// 驱动卸载例程
VOID DriverUnload(PDRIVER_OBJECT driverObject) {
    UNREFERENCED_PARAMETER(driverObject);
    DbgPrint("Driver unloaded.\n");
}

// 驱动入口点
NTSTATUS DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath) {
    UNREFERENCED_PARAMETER(registryPath);

    // 设置驱动卸载例程
    driverObject->DriverUnload = DriverUnload;

    while (TRUE) {
        // 读取0x66端口当前值
        ULONG currentPort66Value = ReadPort(0x66);

        // 检查0x66端口值是否变化
        if (currentPort66Value != previousPort66Value) {
            DbgPrint("Port 0x66 value changed from 0x%X to 0x%X\n", previousPort66Value, currentPort66Value);

            // 读取0x62端口的值并输出
            ULONG port62Value = ReadPort(0x62);
            DbgPrint("Port 0x62 value is 0x%X\n", port62Value);

            previousPort66Value = currentPort66Value;
        }
    }

    return STATUS_SUCCESS;
}
