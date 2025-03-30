# kaktus

### 未正式发布前不要使用，不保证接口稳定性

安装方式(debugx64)：
- 安装vulkan
- c++17
- 调试-环境：bin vulkan/bin
- 包含目录：include vulkan/include
- 库目录：lib vulkan/lib
- 预处理器：JPH_FLOATING_POINT_EXCEPTIONS_ENABLED;JPH_DEBUG_RENDERER;JPH_PROFILE_ENABLED;JPH_OBJECT_STREAM;JPH_USE_AVX2;JPH_USE_AVX;JPH_USE_SSE4_1;JPH_USE_SSE4_2;JPH_USE_LZCNT;JPH_USE_TZCNT;JPH_USE_F16C;JPH_USE_FMADD
- 附加依赖项：vsgd.lib;vsgXchanged.lib;vulkan-1.lib;joltb.lib;libtcc64.lib

星历基于父Body，模型基于Body

传输Body使用string在Bodies中查询std::hashMap<std::string,std::shard_ptr<Boby>>

时间使用iTime表示

ModelType和对应的class Data需要讨论

## EphomerisType
- tabulated
- kepler
- aprx_pos_planets
- const
- custom
## Ephomeris
- frame
- starTime
- endTime
- getState(time)
## Body
- type:celestial

//

## kaktus/include/
- util.hpp 工具
- util.h
- util
	- ArrayList.hpp 动态有序数组
- math.hpp 隐函数求解
- math.h
- math
	- Intergators.hpp 数值积分
	- IntEvent.hpp 积分触发器
	- Interpolators.hpp 插值
- astro
	- Body.hpp
	- Bodies.hpp
	- Ephemeris.hpp 各种星历
	- model 杂项
- craft
	- resource 资源
	- phycial 物理
- kos 传感和控制
	- e
	- p
- gis 地形工具
- animtion 动画化
- other 杂项