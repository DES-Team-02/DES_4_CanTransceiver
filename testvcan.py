import can
import time
import random  # 무작위 숫자 생성을 위해 추가

# 가상 CAN 인터페이스 설정
bus0 = can.interface.Bus(channel='vcan0', bustype='socketcan')
bus1 = can.interface.Bus(channel='vcan1', bustype='socketcan')

try:
    while True:
        # 무작위 데이터 생성: 0~255 범위 내의 숫자 8개
        data = [random.randint(0, 255) for _ in range(8)]
        message = can.Message(arbitration_id=0x123, data=data, is_extended_id=False)

        # vcan0으로 메시지 전송
        bus0.send(message)
        print(f"Message sent to vcan0: {data}")

        # vcan1으로 메시지 전송
        bus1.send(message)
        print(f"Message sent to vcan1: {data}")

        # 잠시 대기
        time.sleep(1)

except KeyboardInterrupt:
    # Ctrl+C가 눌리면 프로그램 종료
    print("Program terminated by user")

finally:
    # 버스 닫기
    bus0.shutdown()
    bus1.shutdown()
