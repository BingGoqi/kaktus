#pragma once
typedef struct jstrNode
{
	size_t size;
	int hash;
	char* str;
}jstrNode;
typedef void* jstr;
namespace csson {
	typedef struct sptr
	{
		unsigned int use;
		void* ptr;
	}sptr;
	enum defaltype
	{
		nul = 0, bit64, ptr, shader, other
	};
	enum jsontype
	{
		j_nul = other + 1, j_bool, j_int, j_string, j_obj, j_array, j_double, other
	};
	class cssonObj
	{
	private:
		cssonObj * perv, * next, * cData;
		int type;
		int key;
	public:
		cssonObj(cssonObj* perv, cssonObj* next, cssonObj* cData, int type=0, int key=0) :perv(perv), next(next), cData(cData), type(type), key(key) {};
		cssonObj(cssonObj* perv=nullptr, cssonObj* cData=nullptr, int type=0, int key=0) { cssonObj(perv, nullptr, cData, type, key); }
		~cssonObj()
		{
			if (this->type == defaltype::shader)
				if (0 == --this->cData->key)
					this->cData->~cssonObj();
			this->cData->~cssonObj();

		}
		cssonObj& setVal(long long val)
		{
			this->cData = (cssonObj*)val;
			return *this;
		}
		cssonObj& setVal(void* val)
		{
			this->cData = (cssonObj*)val;
			return *this;
		}
		cssonObj& setshader(cssonObj* val)
		{
			this->cData = (cssonObj*)val;
			return *this;
		}
		cssonObj& setType(int type)
		{
			this->type = type;
			return *this;
		}
		cssonObj& setKey(int key)
		{
			this->key = key;
			return *this;
		}
		cssonObj& addChildStar(long long val,int type,int key=0)
		{
			cssonObj child = cssonObj(this,(cssonObj*)val,type,key);
			this->cData = &child;
			return child;
		}
		cssonObj& addChildStar(void* val, int type, int key = 0)
		{
			cssonObj child = cssonObj(this, (cssonObj*)val, type, key);
			this->cData = &child;
			return child;
		}
		cssonObj& addChildList(long long val, int type, int key = 0)
		{
			cssonObj child = cssonObj(this->perv, (cssonObj*)val, type, key);
			this->next = &child;
			return child;
		}
		cssonObj& addChildList(void* val, int type, int key = 0)
		{
			cssonObj child = cssonObj(this->perv, (cssonObj*)val, type, key);
			this->next = &child;
			return child;
		}
		cssonObj& addChileEnd()
		{
			return *this->perv;
		}
	};
}