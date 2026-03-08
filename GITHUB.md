# 🛠️ GitHub 협업 및 개발 정책 (GitHub Policy)

## 1. Branch Naming Convention
모든 작업은 Issue 기반으로 브랜치를 생성한다.

* `main` : 안정 브랜치 (배포용)
* `feature/#이슈번호-기능명` : 새로운 기능 개발
* `fix/#이슈번호-버그명` : 버그 및 오류 수정
* `docs/#이슈번호-문서명` : 문서 작업

**예시:**
* `feature/#12-manual-childlock`
* `fix/#18-speed-signal-validation`
* `docs/#05-update-development-doc`

## 2. Commit Message Convention
커밋 메시지는 아래 Prefix를 명확히 사용하여 작성한다.

* `feat:` 새로운 기능 추가
* `fix:` 버그 수정
* `docs:` 문서 수정
* `refactor:` 코드 구조 리팩토링 (기능 변화 없음)
* `test:` 테스트 코드 추가/수정
* `chore:` 빌드/환경 설정 변경

**예시:**
* `feat: add automatic child lock activation logic`
* `fix: block unlock command while vehicle is moving`
* `docs: update DEVELOPMENT.md`

## 3. Pull Request Rules
* `main` 브랜치 직접 push는 엄격히 금지한다.
* 모든 코드 변경은 Pull Request를 통해서만 병합한다.
* Pull Request 생성 전 로컬에서 빌드와 단위 테스트를 수행해야 한다.
* PR 설명에는 변경 목적, 주요 변경 내용, 테스트 결과를 포함한다.
* 최소 1인 이상의 팀원 리뷰(Approve) 후 병합을 진행한다.

## 4. Definition of Done (작업 완료 기준)
아래 조건을 모두 만족해야 해당 작업이 완료된 것으로 간주한다.

- [ ] 관련 Issue가 등록되어 있을 것
- [ ] 작업 브랜치에서 구현이 완료되었을 것
- [ ] 코드가 정상적으로 빌드될 것 (CMake/GCC)
- [ ] 단위 테스트(Google Test)가 통과할 것
- [ ] 정적 분석(Cppcheck) 결과 치명적인 오류가 없을 것
- [ ] 관련 문서(SwDD, Wiki 등)가 업데이트될 것
- [ ] Pull Request 리뷰가 완료될 것

## 5. CI Policy
GitHub Actions는 `push` 및 `pull_request` 이벤트 발생 시 자동 실행된다.

**기본 검사 항목:**
1. Build
2. Unit Test (Google Test)
3. Static Analysis (Cppcheck)
4. Coverage Measurement (Gcov/Lcov)

*💡 CI 결과가 실패(실패 표시 ❌)한 경우 `main` 브랜치에 절대 병합하지 않는다.*
