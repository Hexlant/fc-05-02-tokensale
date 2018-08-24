#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/currency.hpp>

using namespace std;
using namespace eosio;

class tokensale : public eosio::contract {
  public:
    using contract::contract; 
    
    /// @abi action    
    void createsale(const account_name account, uint32_t start_time, uint32_t end_time, const asset& salecap, uint64_t rate) {
      // 컨트랙트 접근권한이 있는지 확인
      //require_auth(_self);
      sales saleinfo(_self, _self);
     
      // 세일 정보가 하나도 없는 경우 테이블에 ROW 생성
      if (saleinfo.begin() == saleinfo.end()) {
	saleinfo.emplace(_self, [&](auto& info) {
	  info.account = account;
          info.start_time = start_time;
          info.end_time = end_time;
          info.salecap = salecap;
          info.sold = salecap;
          info.sold.amount = 0;
          info.rate = rate;
        });
      } else {
        saleinfo.modify(saleinfo.begin(), 0, [&](auto& info) {
          info.account = account;
          info.start_time = start_time;
          info.end_time = end_time;
          info.salecap = salecap;
          info.sold = salecap;
          info.sold.amount = 0;
          info.rate = rate;
        });
      }
    }
  /// @abi table sale i64
  struct sale {
    account_name account;  			// 판매할 토큰의 컨트랙트 계정
    uint32_t start_time;      			// 시작일
    uint32_t end_time;      			// 종료일
    asset salecap;              		// 판매 하려는 토큰의 수량
    asset sold;                 		// 현재까지 판매된 토큰의 수량
    uint64_t rate;      			// 1 EOS 당 지급할 토큰의 수량
    uint64_t primary_key() const {return account;}
    EOSLIB_SERIALIZE(sale,(account)(start_time)(end_time)(salecap)(sold)(rate))
  };
  typedef multi_index<N(sale),sale> sales;

};

EOSIO_ABI( tokensale, (createsale) )
