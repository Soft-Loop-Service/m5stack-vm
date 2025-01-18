

#ifndef __BYTECODE_D
#define __BYTECODE_D
#include "./definition.hpp"

namespace Bytecode
{
    typedef int opcr;
    namespace Opecode
    {
        const opcr d_int = 100;
        const opcr d_str = 200;
        const opcr d_float = 300;
        const opcr d_pointer = 400;   // ポインタ
        const opcr d_reference = 500; // 参照 インスタンス化したものを含む
        const opcr d_html_dom = 600;
        const opcr d_function = 700; // 関数
        const opcr d_class = 800;    // クラスやコンポーネントなど、インスタンス化できるもの。
        const opcr d_boolean = 900;

        // opcr resolvOpecrType(String type, vstring token_class_type);
        // opcr resolvOpecrType(String type);

        const opcr value_null = 0;
        const opcr bool_true = 5;
        const opcr bool_false = 6;

        // ******

        // stack関連
        const opcr push = 10; // スタックに数値もしくは文字列を積む 第1引数は型 第2引数は値

        const opcr push_function = 11; // 関数をスタックに積む 第1引数は関数のインデックス

        const opcr pop = 15; // スタックから削除。ただしどこにも記録しない。

        // 関数呼び出し
        // この命令より前に、スタックに引数→対象関数を積んでおく
        const opcr s_invokevirtual = 20;

        // コンストラクタ呼び出し
        // 第2引数はクラスのインデックス
        const opcr s_invokevirtual_constructor = 21;

        // インスタンス化したものをスタックに積む（インスタンス化のみ）
        // 第2引数はクラスのインデックス
        const opcr s_instance = 22;

        // スタックトップにあるスコープをPOPし、その中にある変数を探す。存在する場合はスタックに積む
        // 第二引数以降は 候補となる   TYPE NAME の繰り替えし
        const opcr s_inside = 23;

        // 組み込み関数呼び出し
        // 第1引数は特殊関数のID
        // 引数の数はVM側で決める
        const opcr s_special_invokevirtual = 24;

                // ローカル変数関連

        // オペランドスタック → ローカル変数
        // 第1引数は型
        // 第2引数はローカル変数のインデックス (格納先)
        const opcr s_store = 30;

        // ローカル変数 → オペランドスタック
        // 第1引数は型
        // 第2引数はローカル変数のインデックス
        const opcr s_load = 40;

        const opcr s_jump = 50;        // 第1引数はラベルID
        const opcr s_label_point = 51; // 第1引数はラベルID

        const opcr c_add = 100;
        const opcr c_subtraction = 101;
        const opcr c_multiplication = 102;
        const opcr c_division = 103;

        // 比較
        // 第1引数はfalse label ID
        const opcr s_if_acmpeq = 111; //== (オブジェクト参照)
        const opcr s_if_acmpne = 112; //!= (オブジェクト参照)
        const opcr s_if_icmpeq = 113; // ==
        const opcr s_if_icmpne = 114; // !=
        const opcr s_if_icmpge = 115; // >=
        const opcr s_if_icmpgt = 116; // >
        const opcr s_if_icmple = 117; // <=
        const opcr s_if_icmplt = 118; // <

        // スタックトップにある値がtrueかfalseかで分岐
        const opcr s_if_true = 120;
        const opcr s_if_false = 121;

        // 第1引数は directly_index
        const opcr head_start_function = 240;
        const opcr head_end_function = 241;

        // 第1引数は directly_index
        const opcr head_start_class = 242;
        const opcr head_end_class = 243;

        // 第1引数 local_stack index
        // 第2引数 scope_type
        // 第3引数 directly_index
        // 第4引数 parent_index
        // 第5引数 以降変数のインデックス(TYPE,D_INDEX)
        const opcr head_value_definition = 244;

        const opcr head_start_global_scope = 245;
        const opcr head_end_global_scope = 246;

        const opcr program_start = 250;
        const opcr program_end = 251;
        // const opcr label_end = 244;   // 第二引数はラベルID
        /*
        1. **if_acmpeq**: スタック上の2つのオブジェクト参照を比較し、等しい場合に分岐します。
        2. **if_acmpne**: スタック上の2つのオブジェクト参照を比較し、等しくない場合に分岐します。
        3. **if_icmpeq**: スタック上の2つの整数を比較し、等しい場合に分岐します。
        4. **if_icmpge**: スタック上の2つの整数を比較し、最初の値が2番目の値以上の場合に分岐します。
        5. **if_icmpgt**: スタック上の2つの整数を比較し、最初の値が2番目の値より大きい場合に分岐します。
        6. **if_icmple**: スタック上の2つの整数を比較し、最初の値が2番目の値以下の場合に分岐します。
        7. **if_icmplt**: スタック上の2つの整数を比較し、最初の値が2番目の値より小さい場合に分岐します。
        8. **if_icmpne**: スタック上の2つの整数を比較し、等しくない場合に分岐します。
        9. **ifeq**: スタック上の整数が0の場合に分岐します。
        10. **ifge**: スタック上の整数が0以上の場合に分岐します。
        11. **ifgt**: スタック上の整数が0より大きい場合に分岐します。
        12. **ifle**: スタック上の整数が0以下の場合に分岐します。
        13. **iflt**: スタック上の整数が0より小さい場合に分岐します。
        14. **ifne**: スタック上の整数が0でない場合に分岐します。
        15. **ifnonnull**: スタック上のオブジェクト参照がnullでない場合に分岐します。
        16. **ifnull**: スタック上のオブジェクト参照がnullの場合に分岐します。
        17. **iinc**: ローカル変数に対して定数値を加算します。この命令は、ループカウンタのインクリメントなどに使用されます。
    */
    }
}

#endif