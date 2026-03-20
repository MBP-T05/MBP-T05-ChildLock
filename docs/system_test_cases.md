# 🧪 기능 테스트 케이스 (Functional Test Case: ReqTest.md)

이 문서는 `Software_Requirement_Specification.md` 및 설계 다이어그램을 기반으로 작성된 기능 테스트 케이스를 관리합니다.

## 📋 테스트 개요
- **대상 시스템**: 전자식 차일드 락 시스템 (Electronic Child Lock System)
- **테스트 목적**: 요구사항 명세서(SRS)에 정의된 모든 기능(UC-1 ~ UC-7) 및 예외 상황에 대한 ASPICE/ISO26262 기반의 전문적 검증
- **참조 문서**:
    - [[SwRW]]
    - [[SwDD]]

## 🏗️ Test Basis
각 테스트 케이스는 구조적/기능적 커버리지를 보장하기 위해 아래 절을 근거로 작성되었습니다.
- **SRS UC-1 ~ UC-7**: 요구사항별 전이 및 인터페이스 조건
- **ISO 26262-8.14**: Equivalence Partitioning, Boundary Value Analysis, Decision Table, State Transition, Fault Injection 적용

## 🗂️ 테스트 케이스 목록 (System Test Cases)

SRS 요구사항(UC-1 ~ UC-7)과 ISO 26262/ASPICE 표준을 엄격히 준수하여 설계된 테스트 케이스 요약입니다. AI 검토를 통해 추가된 고품질 신뢰성·안전성 시나리오를 포함합니다.
상세 내용은 아래 링크를 참조하십시오.

| ID | 요구사항 ID | 테스트 항목 | 적용 기법 | ASIL 등급 (우선순위) |
| :--- | :--- | :--- | :--- | :--- |
| TC-CHL-001 | UC-1 | [EP] 정차 중 수동 스위치 제어 정상 동작 | Equivalence Partitioning | QM (High) |
| TC-CHL-002 | UC-1 | [BVA] 스위치 채터링 디바운싱 - Off-limit (49ms) | Boundary Value Analysis | QM (High) |
| TC-CHL-003 | UC-1 | [BVA] 스위치 채터링 디바운싱 - On-limit (50ms) | Boundary Value Analysis | QM (High) |
| TC-CHL-004 | UC-1 | [Safety] 주행 중 해제 차단 - On-limit (3.0km/h) | Boundary Value Analysis | ASIL B (High) |
| TC-CHL-005 | UC-1 | [BVA] 주행 중 해제 허용 - Off-limit (2.9km/h) | Boundary Value Analysis | ASIL B (High) |
| TC-CHL-006 | UC-1 | [Fail-Safe] ECU 통신 단절 재전송 및 안전상태 | Fault Injection | QM (High) |
| TC-CHL-007 | UC-2 | [BVA] 속도 초과 시 자동잠금 미동작 - 14.9km/h | Boundary Value Analysis | QM (High) |
| TC-CHL-008 | UC-2 | [BVA] 속도 초과 시 자동잠금 동작 - 15.0km/h | Boundary Value Analysis | QM (High) |
| TC-CHL-009 | UC-3 | [Safety] 충돌시 비상 해제 (복합조건 = True) | Decision Table | ASIL B (High) |
| TC-CHL-010 | UC-3 | [Safety] 충돌 단일조건 무시 (오동작 방지) | Decision Table | ASIL B (High) |
| TC-CHL-011 | UC-4 | [ST] ECU 상태(Init->Normal->Lock) 핸들 차단 | State Transition | QM (High) |
| TC-CHL-012 | UC-5 | [Pairwise] 후방 객체(거리/속도/시간) 복합 감지 | Pairwise | QM (High) |
| TC-CHL-013 | UC-5 | [EP] 후방 센서 노이즈/반복 경고 히스테리시스 | Equivalence Partitioning | QM (Medium) |
| TC-CHL-014 | UC-6 | [DT] 승객 점유 및 잠금 OFF 출발 시 시나리오 | Decision Table | QM (Medium) |
| TC-CHL-015 | UC-6 | [Fail-Safe] 점유 센서 에러 발생 시 보호 로직 | Fault Injection | QM (Medium) |

> [!NOTE]
> **AI 확장 범위**: AI(Gemini 2.0 Flash) 분석을 통해 기존 시나리오에 Fault Injection(결함주입), 차단/응답 응계값 분석(BVA - On/Off-point), 히스테리시스/디바운싱 엣지 케이스를 추가 보강하였습니다. ASIL B 수준의 핵심 안전 로직(해제 차단 및 비상해제)을 명확히 분리하여 ASPICE SWE.6 기준을 충족했습니다.

---

## 📊 상세 결과
상세 시스템 테스트 케이스 명세서는 아래 링크에서 확인할 수 있습니다.
- [[System_Test_Cases]]

---

