local autoTable = {
	"欣荣","睿识","和风","文石","正诚","天元","正德","金鑫","宏远","鸿振","安民","心水","凯唱","浩宕","蕴藉","高驰","阳焱","博赡","高歌","向阳",
	"修雅","阳羽","思聪","英朗","浩大","彬彬","俊郎","博学","修为","乐天","承基","温韦","高韵","阳曜","鸿畅","理全","意远","俊贤","明志","朋义",
	"元青","高明","安国","元龙","欣悦","元明","元驹","元基","元化","元德","元白","俊悟","煜祺","锐进","玉宇","玉轩","玉书","嘉良","俊智","玉山",
	"建元","宜民","濮存","康裕","嘉年","立诚","阳华","弘厚","嘉祥","英资","凯风","玉树","高雅","俊能","彭湃","阳云","飞翼","良哲","鸿羽","温纶",
	"奇胜","向晨","鸿云","奇思","良弼","兴怀","明知","文山","弘方","翰池","文瑞","鹏运","温文","光耀","昊焱","高洁","文康","和安","明诚","康德",
	"天佑","温茂","君浩","俊雄","才英","安宁","恺歌","鸿祯","浩歌","景曜","坚成","高峰","俊材","修明","飞沉","修诚","雅珺","烨赫","飞宇","博厚",
	"刚豪","凯定","永宁","弘和","鸿朗","欣怡","修筠","康宁","坚秉","宏达","茂学","明煦","鹏涛","学林","嘉许","宏放","嘉庆","鹏鹍","同和","伟志",
	"信瑞","飞星","俊雅","安然","翰翮","高飞","弘新","康安","鹏云","项禹","琪睿","和泽","泰初","景同","翰藻","翰采","涵润","宾鸿","和玉","彭薄",
	"元甲","俊健","修能","俊德","坚壁","嘉泽","翰音","博超","景龙","博瀚","兴旺","阳冰","晗日","文虹","飞语","安翔","博实","智敏","越彬","德华",
	"俊友","浩言","星宇","雨石","子明","浩阔","锐锋","锐阵","嘉茂","浩淼","勇男","伟毅","坚诚","安怡","良骥","嘉玉","嘉歆","乐安","宏硕","苑杰",
	"永丰","健柏","永福","文华","彭彭","阳夏","德辉","高峯","才良","乐康","睿广","高朗","茂典","子晋","锐藻","英武","英韶","乐人","良奥","嘉瑞",
	"欣然","明亮","浩气","乐容","彬炳","阳荣","晋鹏","华奥","立人","欣嘉","泰清","高丽","鸿哲","和雅","弘济","元武","奇文","涵忍","嘉德","祺瑞",
	"文曜","永安","承德","博涛","高懿","学文","俊楚","飞光","承载","绍元","和歌","和通","高格","华辉","博文","高寒","永年","开霁","和蔼","俊楠",
	"良朋","文光","建章","君昊","成仁","璞瑜","星海","经亘","文滨","光济","俊晤","志行","嘉纳","鸿轩","学名","展鹏","经艺","德业","敏才","鸿文",
	"雨信","熠彤","力勤","正豪","乐生","鹏程","成双","宇荫","景胜","溥心","承望","俊杰","同方","奇玮","烨磊","鸿信","光远","飞鸣","康健","浩瀚",
	"昊乾","圣杰","天工","泽语","弘壮","天干","承平","飞羽","乐池","阳炎","乐志","宜年","成化","乐咏","高轩","和璧","鹏举","建华","嘉慕","博艺",
	"和悌","宏邈","彭泽","浩荡","文翰","飞翔","文耀","元凯","振国","飞捷","建弼","乐音","天泽","成礼","伟祺","锐智","庆生","俊茂","博远","高逸",
	"昊然","子石","德佑","博简","宏义","元良","俊名","昂雄","烨烁","睿思","成济","弘伟","烨煜","智渊","飞翰","宏深","鹏翼","宏博","景焕","星渊",
	"咏歌","咏德","文彬","乐圣","永贞","永逸","光熙","敏博","永新","永望","永思","斯伯","永寿","奇略","永康","永嘉","向明","鸿光","星辰","永怡",
	"阳飙","雅志","思淼","高原","鸿彩","志文","良工","雅健","伟诚","鹏飞","志强","安平","天纵","景铄","光明","同化","浩渺","浩初","雪风","雪峰",
	"同济","同光","嘉祯","天韵","博容","浩浩","天睿","天瑞","刚洁","宜然","阳晖","天禄","德海","景山","睿明","英卓","翔飞","宏茂","奇正","思远",
	"浩波","和硕","向文","嘉实","天骄","文赋","乐逸","阳德","承嗣","伟才","绍钧","信鸥","华茂","建中","凯康","恺乐","明哲","光辉","昊空","高芬",
	"飞龙","玉泉","明旭","英哲","成龙","英毅","英逸","俊誉","英耀","翰墨","英勋","飞驰","英叡","英睿","乐贤","毅然","冠宇","安易","英杰","英豪",
	"乐成","文柏","景明","乐意","烨然","学真","英才","学义","彭越","宏恺","伟博","学博","俊迈","鸿福","绍辉","建木","弘盛","彭勃","滨海","俊才",
	"才哲","德厚","元纬","德润","思博","和悦","晟睿","光霁","睿达","和豫","睿聪","高阳","宏逸","锐精","乐水","懿轩","俊彦","星华","和煦","苑博",
	"德明","良吉","新立","宾实","正志","宏浚","昊英","飞虎","俊爽","安宜","涵映","康顺","高远","志泽","修文","嘉澍","阳朔","明辉","涵亮","华荣",
	"勇毅","英锐","元洲","华藏","俊人","嘉熙","正平","波鸿","弘光","敏学","子瑜","承福","嘉珍","建茗","力夫","心思","璞玉","波涛","泰平","修远",
	"康乐","茂勋","雨伯","承教","兴运","祺然","和平","宏才","华皓","兴腾","新荣","博延","凯凯","致远","靖琪","绍祺","高兴","承恩","元亮","玉宸",
	"宏阔","天宇","和同","翔宇","宏伟","修谨","嘉石","飞章","英喆","和惬","鹏海","乐山","天翰","文星","承悦","光誉","英博","德元","学海","伟彦",
	"玉堂","刚毅","茂才","晗昱","飞飙","宏儒","泽宇","志新","英华","自强","志尚","高昂","星波","明德","敏智","良平","建修","自明","立轩","浩穰",
	"华池","波光","俊发","英光","英范","志国","华采","英发","建同","修洁","英飙","昊苍","文林","天逸","斌蔚","项明","凯复","高岑","弘大","嘉赐",
	"子安","浩慨","季同","康复","信鸿","修伟","开朗","欣怿","鸿德","锐意","星洲","安晏","星纬","星阑","季萌","兴思","锐逸","天路","飞舟","德寿",
	"奇希","烨霖","良翰","奇伟","奇邃","安福","奇水","博涉","经赋","浩漫","承运","彭祖","乐语","嘉颖","明远","奇逸","飞鸿","高谊","明达","敏叡",
	"泰和","锐达","昊穹","涵容","建树","理群","和昶","星驰","巍昂","浩旷","兴修","乐章","宜春","兴贤","哲瀚","子昂","光华","鹏池","兴平","茂德",
	"承泽","承天","勇锐","经略","嘉悦","康适","和怡","黎明","高卓","宏伯","雪松","弘雅","乐邦","翰海","鸿风","元恺","高超","俊哲","博易","良俊",
	"飞航","博雅","才艺","宏壮","和宜","文成","弘深","高扬","承安","文彦","国安","伟泽","伟晔","伟懋","华容","伟茂","涵煦","巍奕","成业","鸿博",
	"承志","子轩","兴为","子濯","光临","雅惠","子真","子琪","子平","子墨","良畴","子默","修真","智志","智明","志用","志业","承宣","志学","德水",
	"文轩","宜修","俊侠","俊艾","修平","凯捷","宏爽","睿德","飞白","和颂","华翰","德运","永长","阳秋","锐志","伟兆","明杰","修贤","乐正","安志",
	"欣可","修然","力学","学民","华美","飞鸾","高澹","丰羽","元忠","鸿运","鸿雪","力强","浩壤","雅昶","弘化","鹏赋","修永","弘益","星晖","俊捷",
	"泰宁","景辉","良才","新觉","俊美","锐利","炫明","向荣","文敏","立果","立群","和正","宏盛","天罡","雅逸","华灿","嘉致","星光","华清","玉石",
	"文乐","飞跃","和韵","皓轩","德义","建业","智鑫","乐心","子民","烨烨","昊昊","鸿宝","成周","坚白","高畅","志义","高邈","茂实","浩广","阳泽",
	"阳舒","涵涤","俊力","明智","永言","荣轩","瑾瑜","嘉树","天空","安顺","黎昕","建安","成和","宏富","蕴和","兴德","弘扬","俊驰","向笛","承允",
	"自珍","乐和","泰然","高翰","弘懿","风华","文德","嘉容","景澄","俊弼","英彦","泰鸿","浩思","弘业","祺祥","宾白","睿好","志专","修杰","星雨",
	"欣德","修齐","嘉佑","阳文","嘉平","立辉","弘毅","凯泽","兴言","曾琪","元勋","天华","飞雨","经国","炎彬","宜人","新知","鸿才","弘阔","振海",
	"自怡","力言","星汉","星火","升荣","奇致","阳成","智勇","峻熙","意蕴","睿博","瀚漠","文昌","锐思","永元","丰茂","光亮","彬郁","兴邦","玉成",
	"英悟","雅达","和顺","志明","英卫","作人","志诚","明俊","阳曦","正真","正谊","华晖","正业","建明","飞尘","兴发","正信","建德","正祥","奇志",
	"成益","康平","烨伟","逸春","俊民","乐童","飞掣","弘博","逸明","开畅","泰华","逸仙","浦和","光赫","泰河","文斌","蕴涵","嘉荣","烨熠","朋兴",
	"文宣","兴国","正阳","和洽","开诚","兴朝","睿才","俊英","明轩","昂然","兴昌","凯乐","兴安","斌斌","信然","阳煦","信厚","俊喆","鸿晖","睿慈",
	"乐欣","高旻","弘量","俊豪","景天","宏旷","承颜","经纬","涵意","兴业","建本","博达","国源","建白","嘉志","嘉福","博明",
	"瀚海","巍然","睿诚","心远","阳平","锦程","宏峻","阳辉","飞文","安澜","冠玉","嘉誉","成文","俊风","星剑","阳伯","高义","鸿畴","阳波","阳飇",
	"鸿达","鸿羲","旭尧","景福","涵畅","凯安","刚捷","飞昂","德惠","明喆","雅懿","宏毅","明朗","浩邈","康伯","浩然","意智","翰飞","乐悦","锐立",
	"德庸","英纵","玉龙","雨泽","涵育","茂材","雨星","雨华","才捷","宇文","宇寰","瀚玥","凯歌","宇航","宇达","鸿卓","勇捷","咏志","咏思","高峻",
	"安康","祺福","良策","温书","俊达","意致","鸿熙","兴庆","弘致","星河","嘉禧","新翰","俊拔","兴文","嘉言","鸿禧","德本","宏大","浩涆","成荫",
	"正文","正青","正浩","哲彦","星文","安和","浦泽","哲圣","哲茂","宏胜","力行","彭魄","安邦","元正","涵涵","成天","远航","斯年","元思","康时",
	"和泰","德容","嘉胜","俊明","明珠","鹏鲸","承业","安歌","永昌","兴学","鸿波","康泰","嘉懿","烨华","阳嘉","天赋","阳州","鸿志","飞扬","和畅",
	"新霁","思源","元魁","国兴","建义","开济","德馨","经纶","翰学","天材","天和","鸿飞","高达","晓博","经武","和裕","玉韵","德曜","锐翰","宏朗",
	"睿范","经业","英达","敏达","嘉勋","翰林","宏畅","康盛","飞鹏","修德","俊远","浩博","天成","飞英","兴生","鹏鲲","华彩","涵蓄","阳旭","嘉谊",
	"正奇","俊良","成弘","德宇","光启","鹤轩","正初","德泽","修竹","宏扬","乐游","博裕","建柏","俊逸","高爽","正卿","凯旋","弘亮","奇迈","鹏天",
	"康胜","志勇","英奕","涵衍","高杰","飞翮","俊语","才俊","昊天","嘉木","嘉运","同甫","和光","弘图","温瑜","乐家","正雅","良材","文栋","勇军",
	"茂彦","良骏","星津","元嘉","锐泽","经义","和志","玉泽","鸿煊","永春","弘文","波峻","鸿远","承弼","子实","弘义","康成","开宇","雅畅","乐湛",
}
function GetTable()
	return autoTable
end
