# ProjectPA

프로젝트 PA 의 포트폴리오용 리포지토리로 소스코드만을 올려두었습니다.

주요 코드들에 대한 설명은 아래를 확인해주시면 감사하겠습니다.

## 아이템 사용 ProjectPA

프로젝트 PA 의 포트폴리오용 리포지토리로 소스코드만을 올려두었습니다.

주요 코드들에 대한 설명은 아래를 확인해주시면 감사하겠습니다.

## 아이템

게임 내 아이템은 UObject 로 처음 제작하였지만 현재는 uint32 2개를 가진 구조체로 정보를 주고 받습니다.
[리펙토링 과정을 담은 블로그 글](https://husk321.tistory.com/472) 이 있으니 해당 글에서 자세히 알아볼 수 있습니다.

- [FItemEncodedInfo](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Generals/PATypes.h)에서 해당 구현 사항을 확인할 수 있습니다
  - uint32 에서 각 정보를 가져오록 함수들을 준비하였습니다.
  - SerializeIntPacked‎ 을 활용해 FName의 키를 전송하는 것처럼 작동하도록 적용했습니다.
- FFastArraySerializerItem, FFastArraySerializer 를 상속받아 순서가 중요하지 않은 필드 위 아이템 리스트는 TArray 보다 리플리케이트를 빠르게 할 수 있도록 제작하였습니다.
  - [ItemListTypes.h](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Items/ItemListTypes.h)
- 각 아이템은 아이템의 FName 키를 통해 데이터 테이블에서 정보를 가져와 UI 및 어빌리티에서 동작하게 됩니다.


## Ability 활용

- Ability Set 을 제작해 각 캐릭터에 기본적으로 부여되는 어빌리티들을 한데 모을 수 있습니다.
  - 어빌리티 부여 시 InputTag 를 함께 줘 플레이어에서는 입력에 대응 가능하게, AI 는 이벤트 Tag 에 대응 가능하도록 제작되었습니다.
  - [AbilitySet헤더](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Abilities/PAAbilitySet.h), [AbilitySet cpp](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Abilities/PAAbilitySet.cpp)
- 플레이어의 '아이템 사용' 태그가 들어온다면 라우터 역할을 하는 어빌리티에 아이템 정보를 전송하고 아이템에 맞는 어빌리티를 트리거 합니다.
  - [현재 장착중인 아이템용 라우터 어빌리티](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Abilities/UseEquippingItemAbility.cpp)
  - [UI 등의 입력으로사용한 아이템용 라우터 어빌리티](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Abilities/DirectUseItemAbility.cpp)
  - 이런 라우터 어빌리티들은 Activate 후 바로 EndAbility 를 호출해야 하므로 호출을 누락시키지 않도록 RAII 패턴 객체를 활용했습니다
    - [EndAbility 호출용 RAII 객체](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Abilities/AbilityObjects/ScopedAbilityEnd.h)
   
## MVVM 패턴 적용
단순 MVVM 패턴을 적용할 수도 있지만 에픽의 플러그인을 통해 실 프로젝트에서도 사용이 가능한지를 파악하기 위해 **UMG ViewModel**플러그인을 도입해 활용해 보았습니다.
- 현재는 모든 Widget 에 적용되지는 않았고 아이템 인벤토리/근처 아이템을 보여주는 위젯에 MVVM 패턴을 적용하였습니다.
   - [사용한 ViewModel](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Widgets/ViewModel/ItemListViewModel.cpp)
- 기존 플러그인의 ViewModel 로는 **언제 어디서든 ViewModel을 생성/할당**하기에는 어려운 구조라 새로운 플러그인을 제작하였습니다.
  - [새로 제작한 Subsystem](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Widgets/SubSystem/PAViewModelSubsystem.cpp)
  - 그외 반복되어 Subsystem 의 함수를 부르는 것의 피로도를 낮추기 위해 Util 네임스페이스를 제작해 코드를 간결하게 만들었습니다.
    - PAViewModelUtils Namespace의 [헤더](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Widgets/SubSystem/PAViewModelUtils.h)  [소스코드](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Widgets/SubSystem/PAViewModelUtils.cpp)

## 그 외 항목들
- [Motion Matching을 적용한 Anim Instance](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Animations/HumanAnimInstanceBase.cpp)
- [Lyra 샘플의 CameraMode 적용](https://github.com/HUSK-321/ProjectPA_Source_0901/blob/main/Camera/CameraMode.h)
