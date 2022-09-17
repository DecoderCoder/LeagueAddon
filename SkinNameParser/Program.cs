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
        public string Image { get; set; }
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
                                    character.spells.Add(new Spell() { Name = ability.Value.mScriptName, Image = ability.Value.mSpell.mImgIconName[0] });
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
                for (int i = 0; i < 10; i++)
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
                // Saving all
                Console.WriteLine("Saving all data");
                List<string> skinsDotH = new List<string>();
                foreach (var character in characters)
                {
                    skinsDotH.Add("{");
                    skinsDotH.Add(" std::vector<Skin> heroSkins;");
                    int counter = 0;
                    foreach (var skin in character.skins)
                    {
                        skinsDotH.Add(" Skin skin" + counter.ToString() + ";");
                        skinsDotH.Add(" skin" + counter.ToString() + ".Id = " + skin.Id + ";");
                        foreach(var lang in LANGUAGES)
                        {
                            skinsDotH.Add(" skin" + counter.ToString() + ".Name[\"" + lang + "\"] = \"" + skin.Name[lang] + "\";");
                        }
                        skinsDotH.Add(" skin" + counter.ToString() + ".Chromas = " + (skin.Chromas ? "true" : "false") + ";");
                        skinsDotH.Add(" heroSkins.push_back(skin" + counter + ");");
                        skinsDotH.Add("");
                        counter++;
                    }
                    skinsDotH.Add(" SkinsDB[\"" + character.cdnName + "\"] = heroSkins;");
                    skinsDotH.Add("}");
                    skinsDotH.Add("");
                }
                File.WriteAllLines("Skin.h", skinsDotH);
            }
        }
    }
}
