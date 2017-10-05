DELETE FROM PATTERNS;

INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (1, 'Опасные слова', 0, '(^|\s)(до си''х по''р|ве''ка|то|То|мо''л|ча''са|ну|Ну|что''|Что''|та''м|Та''м|э''то|Э''то|Во''т|во''т|Не то|не то|Вот|Так|не бы''л|не бы''ло|не бы''ли|а то|То е''сть|что|э''то|вот|то е''сть|так|Да|да|так|там|Как|как|А то|Что)[\.,\?\!:\-;"\s]', 0, '#ffa500');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (2, 'БукваЁ3', 0, '[а-я]*[ёЁ]{1}[а-я]+''[а-я]*', 0, '#ffa500');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (3, 'БукваЁ4', 0, '[а-я]+''[а-я]*[ёЁ]{1}[а-я]*', 0, '#ffa500');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (4, 'Двойные ударения', 0, '\S*''\S*''\S*', 0, '#ffa500');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (5, 'Спецсимволы', 0, '[\*{}@#/]+', 0, '#87cefa');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (6, 'Большие слова без ударения', 0, '(^|\s)([А-ЕЖ-Яа-еж-я]{5,})[\.,\?\!…:\-;"\s\{]', 0, '#9acd32');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (7, 'Подсветка пояснений', 0, '#([\sА-Яа-я\.\-;:,"\!\?–—“”«»''’\/)\(\{\}/]+)#', 0, '#ffa500');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (8, 'Кавычки и апострофы подряд', 0, '[''"`]{2,}', 0, '#ffa500');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (9, 'Буква Ё', 0, '[ёЁ]{1}[''"]{1}', 0, '#ffa500');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (10, 'Буква Ё2', 0, '[''"]{1}[ёЁ]{1}', 0, '#ffa500');
INSERT INTO PATTERNS (ID, NAME, "TYPE", PATTERN, COLOR, HEXCOLOR)
              VALUES (11, 'Теги', 0, '<.+?>.*?</.+?>', 0, '#9370db');