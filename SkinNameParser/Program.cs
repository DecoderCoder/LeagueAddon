//using Newtonsoft.Json;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using xNet;

namespace LeagueParser
{
    class Spell
    {
        public string Name { get; set; }
        public string ImageName { get; set; }
        public byte[] ImageBytes { get; set; }
        public string MissileName { get; set; }

    }
    class Skin
    {
        public Dictionary<string, string> Name { get; set; }
        public int Id { get; set; }
        public bool Chromas { get; set; }
    }

    class Character
    {
        public string rawName { get; set; }
        public string cdnName { get; set; }
        public List<Skin> skins { get; set; }
        public List<Spell> spells { get; set; }
    }

    class mImgIconName
    {
        string[] icon { get; set; }
    }

    class mSpell
    {
        public string[] mImgIconName { get; set; }
    }

    class abilityInformation
    {
        [JsonProperty("mSpell")]
        public mSpell mSpell { get; set; }

        public string mScriptName { get; set; }

        [JsonProperty("mCharacterName")]
        public string mCharacterName { get; set; }

        [JsonProperty("attackSpeedRatio")]
        public float attackSpeedRatio { get; set; }
    }

    internal class Program
    {
        const string LAST_PATCH = "12.7.1";
        const string RAW_LAST_PATCH = "latest";
        static string[] LANGUAGES = { "en_US", }; //"ru_RU"
        static List<string> Parses(string start, string Where, string end, bool hideexcept = true)
        {
            Regex g5 = new Regex(start + "(.*?)" + end);
            MatchCollection A = g5.Matches(Where);
            List<string> Maj = new List<string>();
            foreach (Match match in A)
            {
                Maj.Add(match.Groups[1].ToString());
            }
            if (hideexcept && Maj.Count == 0)
                Maj.Add("");
            return Maj;
        }

        static List<string> heroesLinks = new List<string>();
        static object ThreadLocker = new object();

        static List<Character> characters = new List<Character>();
        static List<Spell> summoners = new List<Spell>();

        static void ParseSummonerThread()
        {
            HttpRequest request = new HttpRequest();
            string HTML = request.Get("https://raw.communitydragon.org/latest/game/data/shared/shared.bin.json").ToString();
            //HTML = request.Get("https://raw.communitydragon.org/latest/game/data/spells/icons2d/").ToString();
            var dict = JsonConvert.DeserializeObject<Dictionary<string, abilityInformation>>(HTML);
            var nD = dict.Where((x) => { return x.Value.mScriptName != null && x.Value.mScriptName.Contains("Summoner"); });
            foreach (var d in nD)
            {
                
                if (d.Value.mSpell != null)
                {
                   
                    try
                    {
                        byte[] image = request.Get("https://raw.communitydragon.org/latest/game/data/spells/icons2d/" + Path.GetFileNameWithoutExtension(d.Value.mSpell.mImgIconName[0]).ToLower() + ".png").ToBytes();
                        summoners.Add(new Spell() { Name = d.Value.mScriptName, ImageName = d.Value.mScriptName, ImageBytes = image });
                        Console.WriteLine("Name: " + d.Value.mScriptName);
                    }
                    catch (Exception)
                    {
                    }
                   
                }                
            }
        }

        static void ParseCharacterInfoThread()
        {
            HttpRequest request = new HttpRequest();
            while (heroesLinks.Count > 0)
            {
                string currentHero;
                lock (ThreadLocker)
                {
                    currentHero = heroesLinks.First();
                    heroesLinks.RemoveAt(0);
                }
                Console.WriteLine(currentHero);
                Console.WriteLine();
                string HeroData = String.Empty;
                try
                {
                    HeroData = request.Get("https://raw.communitydragon.org/" + RAW_LAST_PATCH + "/game/data/characters/" + currentHero + "/" + currentHero + ".bin.json").ToString();

                }
                catch (Exception)
                {
                    goto skip;
                }
                Character character = new Character();
                character.cdnName = Parses("mCharacterName\": \"", HeroData, "\"")[0];
                character.rawName = currentHero;

                character.skins = new List<Skin>();
                character.spells = new List<Spell>();
                Dictionary<string, string> SkinData = new Dictionary<string, string>();
                foreach (var lang in LANGUAGES)
                {

                    try
                    {
                        SkinData.Add(lang, request.Get("http://ddragon.leagueoflegends.com/cdn/" + LAST_PATCH + "/data/" + lang + "/champion/" + character.cdnName + ".json").ToString());
                    }
                    catch (Exception e)
                    {
                        goto skip;
                    }
                    //{"id":"84002","num":2,"name":"Infernal Akali","chromas":false}                    
                }

                int[] skinIds = Parses("num\":", SkinData[LANGUAGES[0]], ",").Select((x) => { return int.Parse(x); }).ToArray();
                Dictionary<string, string[]> skinNames = new Dictionary<string, string[]>();
                foreach (var lang in LANGUAGES)
                {
                    var names = Parses("name\":\"", SkinData[lang], "\"");
                    names.RemoveAt(0);
                    skinNames.Add(lang, names.ToArray());

                }
                bool[] skinChromas = Parses("chromas\":", SkinData[LANGUAGES[0]], "}").Select((x) => { return x == "true"; }).ToArray();

                for (int i = 0; i < skinIds.Length; i++)
                {
                    Skin skin = new Skin();
                    skin.Id = skinIds[i];
                    skin.Name = new Dictionary<string, string>();
                    foreach (var lang in LANGUAGES)
                    {
                        skin.Name.Add(lang, skinNames[lang][i]);
                    }
                    skin.Chromas = skinChromas[i];
                    character.skins.Add(skin);
                }

                Dictionary<string, abilityInformation> abilitiesInformation;
                string HTML = String.Empty;
                try
                {
                    HTML = request.Get("https://raw.communitydragon.org/latest/game/assets/characters/" + currentHero + "/hud/icons2d/").ToString();
                    abilitiesInformation = JsonConvert.DeserializeObject<Dictionary<string, abilityInformation>>(HeroData);
                }
                catch (Exception)
                {
                    goto skip;
                }

                List<string> images = Parses("<td class=\"link\"><a href=\"", HTML, "\"", false).ToList();
                if (images.Count > 2)
                {
                    images.RemoveAt(0);
                    foreach (string im in images)
                    {
                        byte[] image = request.Get("https://raw.communitydragon.org/latest/game/assets/characters/" + currentHero + "/hud/icons2d/" + im).ToBytes();
                        foreach (var ability in abilitiesInformation)
                        {
                            if (ability.Value.mSpell != null && ability.Value.mSpell.mImgIconName != null)
                            {
                                if (Path.GetFileNameWithoutExtension(ability.Value.mSpell.mImgIconName[0]).ToLower() == Path.GetFileNameWithoutExtension(im).ToLower())
                                {
                                    character.spells.Add(new Spell() { Name = ability.Value.mScriptName, ImageName = ability.Value.mScriptName, ImageBytes = image });
                                    Console.WriteLine(ability.Value.mScriptName);
                                }
                            }
                        }
                    }
                }
                characters.Add(character);
            skip:
                Console.WriteLine("Skipped");
            }
            threadCount--;
        }

        static Thread[] threads;
        static int threadCount = 0;

        static void Main(string[] args)
        {
            HttpRequest request = new HttpRequest();
            string HTML = request.Get("https://raw.communitydragon.org/" + RAW_LAST_PATCH + "/game/assets/characters/").ToString();
            heroesLinks = Parses("<td class=\"link\"><a href=\"", HTML, "\"").Select((x) => { return x.Substring(0, x.Length - 1); }).ToList().Where(x => { return !(x.StartsWith("tft") || x.StartsWith("bw_") || x.StartsWith("brush") || x.StartsWith(".")); }).ToList();
            if (heroesLinks.Count > 2)
            {
                threads = new Thread[10];
                for (int i = 0; i < threads.Length; i++)
                {
                    threadCount++;
                    threads[i] = new Thread(ParseCharacterInfoThread) { IsBackground = false };
                    threads[i].Start();
                }
                while (threadCount != 0)
                {
                    Console.Title = threadCount.ToString();
                    Thread.Sleep(100);
                }

                ParseSummonerThread();

                // Saving all
                Console.WriteLine("Saving all data");
                List<string> resourcesCpp = new List<string>();
                List<string> ImagesH = new List<string>();

                resourcesCpp.Add("#include \"pch.h\"\r\n\r\n\r\n");
                resourcesCpp.Add("#include \"Resources.h\"");
                resourcesCpp.Add("#include \"Images.h\"");
                resourcesCpp.Add("void Resources::LoadSkinsDB(std::map<string, std::vector<Skin>>& SkinsDB)");
                resourcesCpp.Add(" {");
                foreach (var character in characters)
                {
                    resourcesCpp.Add("{");
                    resourcesCpp.Add(" std::vector<Skin> heroSkins;");
                    int counter = 0;
                    foreach (var skin in character.skins)
                    {
                        resourcesCpp.Add(" Skin skin" + counter.ToString() + ";");
                        resourcesCpp.Add(" skin" + counter.ToString() + ".Id = " + skin.Id + ";");
                        foreach (var lang in LANGUAGES)
                        {
                            resourcesCpp.Add(" skin" + counter.ToString() + ".Name[\"" + lang + "\"] = \"" + skin.Name[lang] + "\";");
                        }
                        resourcesCpp.Add(" skin" + counter.ToString() + ".Chromas = " + (skin.Chromas ? "true" : "false") + ";");
                        resourcesCpp.Add(" heroSkins.push_back(skin" + counter + ");");
                        resourcesCpp.Add("");
                        counter++;
                    }
                    resourcesCpp.Add(" SkinsDB[\"" + character.cdnName + "\"] = heroSkins;");
                    resourcesCpp.Add("}");
                    resourcesCpp.Add("");
                }
                resourcesCpp.Add(" }");

                ImagesH.Add("#pragma once\r\n#ifndef DEBUG\r\nnamespace Images {");
                resourcesCpp.Add("void Resources::LoadImages(Image_Manager & Images_Manager) {");

                foreach (var spell in summoners)
                {
                    resourcesCpp.Add("Images_Manager.AddImage(\"" + spell.Name + "\", (char*)&Images::" + spell.ImageName + ", sizeof(Images::" + spell.ImageName + "));");
                    ImagesH.Add("const unsigned char " + spell.Name + "[] = {");
                    ImagesH.Add(String.Join(", ", spell.ImageBytes.Select((x) => { return "0x" + x.ToString("X2"); })));
                    ImagesH.Add("};");
                }

                foreach (var character in characters)
                {
                    foreach (var spell in character.spells)
                    {
                        resourcesCpp.Add("Images_Manager.AddImage(\"" + spell.Name + "\", (char*)&Images::" + spell.ImageName + ", sizeof(Images::" + spell.ImageName + "));");
                        ImagesH.Add("const unsigned char " + spell.Name + "[] = {");
                        ImagesH.Add(String.Join(", ", spell.ImageBytes.Select((x) => { return "0x" + x.ToString("X2"); })));
                        ImagesH.Add("};");
                    }
                }

                resourcesCpp.Add("}");
                ImagesH.Add("}\r\n#endif");

                File.WriteAllLines("Resources.cpp", resourcesCpp);
                File.WriteAllLines("Images.h", ImagesH);
            }
        }
    }
}
