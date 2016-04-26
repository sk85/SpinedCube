# SQ

スパインドキューブ関連の実験を行うレポジトリです．

<ul>
<li>SPR：</li>
  Shortest Path Routingを行う静的クラスですが，厳密にはminimal expansionの要素を求めることしかやってません
  
<li>Distance：</li>
  2頂点間の距離について実験を行うクラスです．
  次元数を与えると，すべての2頂点の組み合わせの距離を配列に読み込みます．
  計算結果はファイルに保存し，毎回計算は行わなくて良い設計です
  
<li>Node：</li>
  ノードを表すクラスです．
  使う分の演算子しかサポートしていないガバガバ設計です．
  
<li>Expansion：</li>
  expansionの要素集合を保持するクラスです．
  データとしてはNodeの配列ですが，頂点のタイプと隣接決定バイナリのインデックスを保存する形式のほうが賢かったかなと反省しています．
</ul>
