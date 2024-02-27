# Matter Occupancy Sensor Example (ESP32 + EKMC16)
Matter 재실 감지 센서 예제 프로젝트<br>
다음 Matter 클러스터에 대한 코드 구현 방법을 알아본다
- Occupancy Sensing (Cluster ID: `0x0406`)

Software (Matter)
---
1개의 Endpoint가 아래와 같이 생성된다.
1. Endpoint ID 1
    Device Type: Occupancy Sensor (Classification ID: `0x0107`) <br>
    [Clusters]
    - Occupancy Sensing (Cluster ID: `0x0406`) <br>
        [Attributes]
        - Occupancy (Attribute ID: `0x0000`)
        - Occupancy Sensor Type (Attribute ID: `0x0001`)
        - Occupancy Sensor Type Bitmap (Attribute ID: `0x0002`)

Hardware
---
[Pmod PIR](https://digilent.com/reference/pmod/pmodpir/start): Digilent사에서 판매중인 EKMC1601111 `PIR` 모션 센서 탑재 모듈을 사용했다.<br>
<p style="text-align:center"><img src="https://digilent.com/reference/_media/reference/pmod/pmodpir/pmodpir-obl-1000.png" width="300"></p><br>
Motion Detect 신호는 1개의 GPIO(Input)와 연결해야 하며, 핀번호는 /main/include/definition.h에 다음과 같이 정의되어 있다. <br>

```cpp
#define GPIO_PIN_SENSOR_SIGNAL 16
```

SDK Version
---
- esp-idf: [v5.1.2](https://github.com/espressif/esp-idf/tree/v5.1.2)
- esp-matter: [b037340deb2e617b22e9f749346b4f41f6d669b3](https://github.com/espressif/esp-matter/commit/b037340deb2e617b22e9f749346b4f41f6d669b3)
- connectedhomeip: [d38a6496c3abeb3daf6429b1f11172cfa0112a1a](https://github.com/project-chip/connectedhomeip/tree/d38a6496c3abeb3daf6429b1f11172cfa0112a1a)
  - Matter 1.1 released (2023.05.18)
  - Matter 1.2 released (2023.10.23)

Helper Scripts
---
SDK 클론 및 설치
```shell
$ source ./scripts/install_sdk.sh
```
SDK (idf.py) 준비
```shell
$ source ./scripts/prepare_sdk.sh
```

Build & Flash Firmware
---
1. Factory Partition (Matter DAC)
    ```shell
    $ source ./scripts/flash_factory_dac_provider.sh
    ```
2. Configure project
    ```shell
    $ idf.py set-target esp32
    ```
3. Build Firmware
    ```shell
    $ idf.py build
    ```
4. Flash Firmware
    ```shell
    $ idf.py -p ${seiral_port} flash monitor
    ```

QR Code for commisioning
---
![qrcode.png](./resource/DACProvider/qrcode.png)

References
---
[Matter 재실 감지 클러스터 개발 예제 (ESP32)](https://yogyui.tistory.com/entry/PROJ-Matter-%EC%9E%AC%EC%8B%A4-%EA%B0%90%EC%A7%80-%ED%81%B4%EB%9F%AC%EC%8A%A4%ED%84%B0-%EA%B0%9C%EB%B0%9C-%EC%98%88%EC%A0%9C-ESP32)<br>