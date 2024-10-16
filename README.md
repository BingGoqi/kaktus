# kaktus

每个Body都有一个中心坐标系（平动，量纲缩放）

星历基于父Body，模型基于Body

Body中要有星历的所有方法

传输Body使用string在Bodies中查询std::hashMap<std::string,std::shard_ptr<Boby>>

时间使用double表示

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