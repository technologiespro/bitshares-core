#pragma once
#include <graphene/chain/protocol/types.hpp>
#include <graphene/chain/protocol/asset.hpp>
#include <graphene/chain/protocol/authority.hpp>


namespace graphene { namespace chain {

   /**
    *  @defgroup operations Operations
    *  @ingroup transactions Transactions
    *  @brief A set of valid comands for mutating the globally shared state.
    *
    *  An operation can be thought of like a function that will modify the global
    *  shared state of the blockchain.  The members of each struct are like function
    *  arguments and each operation can potentially generate a return value.
    *
    *  Operations can be grouped into transactions (@ref transaction) to ensure that they occur
    *  in a particular order and that all operations apply successfully or
    *  no operations apply.
    *
    *  Each operation is a fully defined state transition and can exist in a transaction on its own.
    *
    *  @section operation_design_principles Design Principles
    *
    *  Operations have been carefully designed to include all of the information necessary to
    *  interpret them outside the context of the blockchain.   This means that information about
    *  current chain state is included in the operation even though it could be inferred from
    *  a subset of the data.   This makes the expected outcome of each operation well defined and
    *  easily understood without access to chain state.
    *
    *  @subsection balance_calculation Balance Calculation Principle
    *
    *    We have stipulated that the current account balance may be entirely calculated from
    *    just the subset of operations that are relevant to that account.  There should be
    *    no need to process the entire blockchain inorder to know your account's balance.
    *
    *  @subsection fee_calculation Explicit Fee Principle
    *
    *    Blockchain fees can change from time to time and it is important that a signed
    *    transaction explicitly agree to the fees it will be paying.  This aids with account
    *    balance updates and ensures that the sender agreed to the fee prior to making the
    *    transaction.
    *
    *  @subsection defined_authority Explicit Authority
    *
    *    Each operation shall contain enough information to know which accounts must authorize
    *    the operation.  This principle enables authority verification to occur in a centralized,
    *    optimized, and parallel manner.
    *
    *  @subsection relevancy_principle Explicit Relevant Accounts
    *
    *    Each operation contains enough information to enumerate all accounts for which the
    *    operation should apear in its account history.  This principle enables us to easily
    *    define and enforce the @balance_calculation. This is superset of the @ref defined_authority
    *
    *  @{
    */

   struct void_result{};
   typedef fc::static_variant<void_result,object_id_type,asset> operation_result;

   struct base_operation
   {
      template<typename T>
      share_type calculate_fee(const T& params)const
      {
         return params.fee;
      }
      void get_required_authorities( vector<authority>& )const{}
      void get_required_active_authorities( flat_set<account_id_type>& )const{}
      void get_required_owner_authorities( flat_set<account_id_type>& )const{}
      void get_impacted_accounts( flat_set<account_id_type>& )const{}
      void validate()const{}

      static uint64_t calculate_data_fee( uint64_t bytes, uint64_t price_per_kbyte );
      static void add_authority_accounts( flat_set<account_id_type>& i, const authority& a )
      {
         for( auto& item : a.account_auths )
            i.insert( item.first );
      }
   };

   ///@}

} }
FC_REFLECT_TYPENAME( graphene::chain::operation_result )
FC_REFLECT( graphene::chain::void_result, )