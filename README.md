# kaktus

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