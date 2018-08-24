#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/transaction.hpp>
#include <eosiolib/crypto.h>
#include <eosiolib/currency.hpp>
#include <eosiolib/time.hpp>

using namespace std;
using namespace eosio;

class tokensale : public eosio::contract {
  public:
    using contract::contract; 
    
    /// @abi action
    //@abi action
    void distribute(uint64_t receiver, uint64_t code) {
      //print("eosio.token : ", N(eosio.token));
      //print(", code : ", code);
      //print("eosio.token : ", eosio.token);
      // EOS토큰이 아닌경우, 이벤트 처리 없이 종료
      //if(code != N(eosio.token)) return;

      // #아래 조건문은 EOS가 아닌경우, 트랜잭션을 리젝 시키기 때문에, 다른 토큰을 받을 수가 없음 
      // eosio_assert(code == N(eosio.token), "I reject your non-eosio.token deposit");

      auto data = unpack_action_data<currency::transfer>();
      if(data.from == _self || data.to != _self) return;

      eosio_assert(data.quantity.symbol == string_to_symbol(4, "EOS"), "EOS 타입이 아닙니다.");
      eosio_assert(data.quantity.is_valid(), "SYMBOL 타입이 맞지 않습니다.");
      eosio_assert(data.quantity.amount > 0, "보낼 수량이 부족합니다.");

      sales saleinfo(_self, _self);
      // 세일 정보가 없다면 트랜재션을 리젝시킴
      eosio_assert(saleinfo.begin() != saleinfo.end(), "등록된 토큰세일이 없습니다.");
      auto info = *(saleinfo.begin());
     
      // EOS를 보내준 계정에 전달할 토큰의 수량 게산
      asset tokens;
      tokens.symbol = info.salecap.symbol;
      tokens.amount = data.quantity.amount * info.rate;
      print(", data.quantity.amount : ", data.quantity.amount);
      print(", info.rate : ", info.rate);
      print(", token_amount : ", tokens.amount);
     
     // 만일 세일 캡 이상으로 토큰을 판매될 경우 트랜잭션 리젝.
     eosio_assert(tokens + info.sold <= info.salecap, "This order exceed salecap");

     // 판매 수량 갱신
     saleinfo.modify( saleinfo.begin(), 0,[&](auto& data) {
       data.sold += tokens;                           
     });

     // 현재 컨트랙트 계정이 가지고 있는 토큰을 전달
     action(
       permission_level{ _self, N(active) },
       info.account, N(transfer),
       std::make_tuple(_self, data.from, tokens, std::string(""))
     ).send();
        
      print(name{data.from}, ", paid:   ", data.quantity," exchanged to : ", tokens );
    }
    /// @abi action
    void now() {
      print("hi");
      //print(time_point_sec::time_point_sec());
    }
    /// @abi action    
    void createsale(const account_name account, uint32_t start_time, uint32_t end_time, const asset& salecap, uint64_t rate) {
      // 컨트랙트 접근권한이 있는지 확인
      require_auth(_self);
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

#define EOSIO_ABI2( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      auto self = receiver; \
      if( action == N(onerror)) { \
         /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
         eosio_assert(code == N(eosio), "onerror action's are only valid from the \"eosio\" system account"); \
      } \
      if( code == self || action == N(onerror) ) { \
         TYPE thiscontract( self ); \
         switch( action ) { \
            EOSIO_API( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
      else { \
         TYPE thiscontract( self ); \
         switch( action ) { \
            case N(transfer): return thiscontract.distribute(receiver, code); \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
} \

EOSIO_ABI2( tokensale, (createsale) )
