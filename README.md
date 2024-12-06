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

##kaktus/include/
- util.hpp
- util.h
- util
	- ArrayList.hpp
- math.hpp
- math.h
- math
	- Intergators.hpp
	- IntEvent.hpp
	- Interpolators.hpp
- astro
	- Body.hpp
	- Bodies.hpp
	- Ephemeris.hpp
	- model
- craft
	- resource
	- phycial
- kos
	- e
	- p
- gis
- vkfw
- animtion
